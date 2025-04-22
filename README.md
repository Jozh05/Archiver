# Архиватор

Простая утилита-архиватор, написанная на C++ с использованием библиотеки **libarchive**.

## 📦 Зависимости

Для сборки проекта необходимо установить библиотеку [libarchive](https://www.libarchive.org/)

Вы можете установить её с помощью [vcpkg](https://github.com/Microsoft/vcpkg):

```bash
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.sh
./vcpkg integrate install
./vcpkg install libarchive
```

Также можно собрать библиотеку из исходников:
[GitHub Libarchive](https://github.com/libarchive/libarchive)

## ⚙️ Сборка проекта

Перед сборкой укажите пути к заголовочным файлам и скомпилированной библиотеке libarchive в файле `CMakeLists.txt`, установив значения переменных:

- `PATH_TO_INCLUDE_DIR`
- `PATH_TO_LINK_DIR`

Для сборки проекта на Linux выполните:

```bash
mkdir build
cd build
cmake ..
make
```
После успешной сборки в папке `build` появится исполняемый файл `archiver`.

Для сборки проекта на Windows выполните:

```bash
mkdir build
cd build
cmake ..
```
Далее используйте любой удобный компилятор для сборки проекта.

## 📘 Инструкция по работе с приложением

Приложение поддерживает 2 комманды:

- **cmp** для архивации файла. Синтаксис: ./archiver cmp <Путь до архивируемого файла> <Название файла архива>
Архив будет создан в той же директории, где находится архивируемый файл.
- **dcmp** для распаковки архива. Синтаксис: ./archiver dcmp <Путь до архива> <Путь до директории, куда будет рапакован архив>
