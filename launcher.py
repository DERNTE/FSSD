import os
import subprocess
import sys
import tkinter as tk
from tkinter import messagebox, scrolledtext

class CProgramLauncher:
    def __init__(self, root):
        self.root = root
        self.root.title("Запуск программ из FSSD")
        self.root.geometry("600x400")

        # Определяем папку, где лежит этот скрипт
        self.base_dir = os.path.dirname(os.path.abspath(__file__))
        self.build_dir = os.path.join(self.base_dir, "build")

        # Создаем папку для скомпилированных файлов, если её нет
        if not os.path.exists(self.build_dir):
            os.makedirs(self.build_dir)

        # Список файлов .c в папке со скриптом
        self.c_files = [f for f in os.listdir(self.base_dir) if f.endswith('.c') and os.path.isfile(os.path.join(self.base_dir, f))]
        self.c_files.sort()  # Сортируем для удобства

        # --- Интерфейс ---
        # Список файлов
        tk.Label(root, text="Выберите программу для запуска:", font=("Arial", 10)).pack(pady=5)

        self.listbox = tk.Listbox(root, selectmode=tk.SINGLE, height=8)
        self.listbox.pack(padx=10, fill=tk.BOTH, expand=True)

        # Заполняем список
        for file in self.c_files:
            self.listbox.insert(tk.END, file)

        # Кнопка запуска
        self.run_button = tk.Button(root, text="Скомпилировать и запустить", command=self.run_selected, bg="#4CAF50", fg="white", pady=5)
        self.run_button.pack(pady=5)

        # Область для вывода логов компиляции/ошибок
        tk.Label(root, text="Вывод компиляции:", font=("Arial", 9)).pack()
        self.log_area = scrolledtext.ScrolledText(root, height=8, state='disabled', bg="#f0f0f0")
        self.log_area.pack(padx=10, pady=5, fill=tk.BOTH, expand=True)

    def log(self, message):
        """Добавляет сообщение в лог-область"""
        self.log_area.config(state='normal')
        self.log_area.insert(tk.END, message + "\n")
        self.log_area.see(tk.END)
        self.log_area.config(state='disabled')
        self.root.update()

    def run_selected(self):
        selection = self.listbox.curselection()
        if not selection:
            messagebox.showwarning("Предупреждение", "Пожалуйста, выберите файл.")
            return

        filename = self.listbox.get(selection[0])
        filepath = os.path.join(self.base_dir, filename)

        # Имя исполняемого файла (без расширения .c)
        executable_name = os.path.splitext(filename)[0]
        # Добавляем .exe для Windows, для Linux/Mac можно без расширения
        if sys.platform == "win32":
            executable_name += ".exe"
        executable_path = os.path.join(self.build_dir, executable_name)

        self.log(f"--- Начинаем компиляцию {filename} ---")

        # Команда компиляции с помощью gcc
        compile_command = ["gcc", filepath, "-o", executable_path]

        try:
            # Запускаем компиляцию и ждем завершения
            process = subprocess.run(compile_command, capture_output=True, text=True, timeout=30)

            if process.returncode != 0:
                self.log(f"Ошибка компиляции:\n{process.stderr}")
                messagebox.showerror("Ошибка компиляции", f"Не удалось скомпилировать {filename}. Подробности в логе.")
                return

            self.log(f"Компиляция успешна! Запускаем {executable_name}...")

            # Запускаем программу
            if sys.platform == "win32":
                subprocess.Popen([executable_path], cwd=self.build_dir, shell=True)
            else:
                subprocess.Popen([executable_path], cwd=self.build_dir)

            self.log(f"Программа {executable_name} запущена.\n")

        except FileNotFoundError:
            self.log("Ошибка: Компилятор 'gcc' не найден. Убедитесь, что он установлен и доступен в PATH.")
            messagebox.showerror("Ошибка", "GCC не найден. Установите компилятор.")
        except subprocess.TimeoutExpired:
            self.log("Ошибка: Компиляция заняла слишком много времени.")
        except Exception as e:
            self.log(f"Непредвиденная ошибка: {e}")

if __name__ == "__main__":
    root = tk.Tk()
    app = CProgramLauncher(root)
    root.mainloop()