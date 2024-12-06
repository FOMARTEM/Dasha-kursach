#include <iostream>
#include <string>
#include <chrono>
#include <random>
#include "unrar.h" // Убедитесь, что у вас есть этот заголовочный файл

using namespace std;
using namespace chrono;

bool tryPassword(const string& password);

int main() {
    string chars = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    string foundPassword = "";
    bool found = false;
    long long attempts = 0;
    string rarname = "test_RAR_for_Dasha.rar";

    auto start = high_resolution_clock::now();
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(0, chars.length() - 1);

    while (!found) {
        string attempt = "";
        for (int i = 0; i < 4; ++i) {
            attempt += chars[distrib(gen)];
        }

        attempts++;
        if (tryPassword(attempt)) {
            foundPassword = attempt;
            found = true;
        }
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    if (found) {
        cout << "Found password: " << foundPassword << endl;
        cout << "Used time: " << duration.count() << " ms" << endl;
    }
    else {
        cout << "Ошибка: Пароль не найден (проверьте ввод)." << endl;
    }

    return 0;
}

bool tryPassword(const string& password) {
    HANDLE hArc;
    string rarname = "test_RAR_for_Dasha.rar";
    RAROpenArchiveDataEx archiveData;
    memset(&archiveData, 0, sizeof(archiveData));

    archiveData.ArcName = const_cast<char*>(rarname.c_str());
    archiveData.OpenMode = RAR_OM_EXTRACT;

    hArc = RAROpenArchiveEx(&archiveData);

    if (hArc == nullptr) {
        cerr << "Не удалось открыть архив." << endl;
        return false;
    }

    RARSetPassword(hArc, const_cast<char*>(password.c_str()));

    int result = RARReadHeader(hArc, nullptr);

    if (result == 0) {
        result = RARProcessFile(hArc, RAR_EXTRACT, nullptr, nullptr);
        RARCloseArchive(hArc);
        return result == 0;
    }

    RARCloseArchive(hArc);
    return false;
}