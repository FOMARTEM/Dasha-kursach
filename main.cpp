#include <iostream>
#include <string>
#include <cstdlib>
#include <chrono>

using namespace std;
using namespace chrono;

typedef struct returns {
    string password;
    long long attempts;
    int found;
}returns;

string getArchivePath() {
    string path;
    cout << "Введите путь к архивированному файлу: ";
    getline(cin, path);
    return path;
}

string getOutputPath() {
    string path;
    cout << "Введите путь, куда будет разархивирован архив: ";
    getline(cin, path);
    return path;
}

string getPassword() {
    string password;
    cout << "Введите пароль: ";
    getline(cin, password);
    return password;
}


int extractArchive(const string& archivePath, const string& outputPath, const string& password) {
    string command = "unrar x -p" + password + " \"" + archivePath + "\" \"" + outputPath + "\"";
    
    // Перенаправление вывода в null
    int result = system((command + " > nul 2>&1").c_str());
    
    //cout << result << endl;
    //2816 - неверный пароль, 2560 не верный путь, 0 всё хорошо
    if (result == 2560) {
        cout << "Неверное указан путь к файлу или место разархивирования" << endl;
        exit(1);
    } 
    return result;
}

returns FindPassword(const string& archivePath, const string& outputPath) {
    //string chars = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    
    string chars = "abcd";
    returns founded = {"", 0, 1};

    //перебор      
        for (char c1 : chars) {
            for (char c2 : chars) {
                for (char c3 : chars) {
                    for (char c4 : chars) {
                        founded.attempts++;
                        string tpassword = "";
                        tpassword += c1;
                        tpassword += c2;
                        tpassword += c3;
                        tpassword += c4;
                        cout << founded.attempts << "%: Пароль - " << tpassword << endl;
                        founded.found = extractArchive(archivePath, outputPath, tpassword);
                        if (founded.found == 0) {
                            founded.password = tpassword;
                            return founded;
                        }
                    }
                }
            }
        }

    return founded;

}

int main() {
    
    // получаем путь к архиву
    string archivePath = getArchivePath();

    // получаем путь куда разархивируем
    string outputPath = getOutputPath();

    // данные
    //returns datas;
    returns datas = {"", 0, 1};

    cout << "Начало поиска пароля:" << endl;

    auto start = high_resolution_clock::now();

    datas = FindPassword(archivePath, outputPath);
    
    //generatePasswords(archivePath, outputPath, "", datas);
    
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);

    if (datas.found == 0) {
        cout << "Найденный пароль: " << datas.password << endl;
        cout << "Количество попыток: " << datas.attempts << endl;
        cout << "Затрачено: " << duration.count() << " ms" << endl;
    } else {
        cout << "Ошибка: Пароль не найден (проверьте ввод)." << endl;
        cout << "Затрачено: " << duration.count() << " ms" << endl;
        return 1;
    }
    
    return 0;
}