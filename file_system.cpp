#include <iostream>
#include <filesystem>
#include <vector>
#include <algorithm>

namespace fs = std::filesystem;

// Проверяет, является ли файл лог-файлом
bool isLogFile(const std::string& filename) {
    // Проверяем, начинается ли имя файла с "prod_"
    // и содержит ли оно расширение ".log"
    return filename.rfind("prod_", 0) == 0 && filename.find(".log") != std::string::npos;
}

// Сравнивает два файла по времени последнего изменения
bool compareFiles(const fs::path& filePath1, const fs::path& filePath2) {
    // Получаем время последнего изменения для каждого файла
    auto time1 = fs::last_write_time(filePath1);
    auto time2 = fs::last_write_time(filePath2);

    if (time1 == time2) {
        // Если время последнего изменения совпадает,
        // сортируем файлы по лексикографическому порядку их путей
        return filePath1.lexically_proximate(fs::current_path()) < filePath2.lexically_proximate(fs::current_path());
    }

    // Возвращаем true, если первый файл был изменен позже второго файла
    return time1 > time2;
}

// Выводит на экран n самых последних лог-файлов в заданной директории
void printRecentLogFiles(const std::string& directoryPath, int count) {
    std::vector<fs::path> logFilePaths;

    // Проходим по всем файлам в заданной директории
    for (const auto& entry : fs::directory_iterator(directoryPath)) {
        // Проверяем, является ли файл лог-файлом
        if (entry.is_regular_file() && isLogFile(entry.path().filename().string())) {
            // Добавляем путь к лог-файлу в вектор
            logFilePaths.push_back(entry.path());
        }
    }

    // Сортируем лог-файлы по времени последнего изменения
    std::sort(logFilePaths.begin(), logFilePaths.end(), compareFiles);

    // Выводим на экран полный путь каждого лог-файла
    for (int i = 0; i < std::min(count, static_cast<int>(logFilePaths.size())); ++i) {
        std::cout << logFilePaths[i].string() << std::endl;
    }
}

int main() {
    setlocale(LC_ALL, "rus");
    std::string directoryPath;
    std::cout << "Введите путь к каталогу: ";
    std::cin >> directoryPath;

    // Выводим на экран 3 самых последних лог-файла в заданной директории
    printRecentLogFiles(directoryPath, 3);

    return 0;
}