#include <iostream>
#include <string>
#include <cstdlib>
#include <chrono>
#include <random>

using namespace std;
using namespace chrono;

typedef struct returns {
    string password;
    long long attempts;
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
    return result;
}

returns FindPassword(const string& archivePath, const string& outputPath) {
    //
    string chars = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    returns founded = {"", 0};
    int found = 1;

    //какой то рандом
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(0, chars.length() - 1);

    //перебор
    while (found != 0) {
        string tpassword = "";
        
        for (int i = 0; i < 4; ++i) {
            tpassword += chars[distrib(gen)];
        }

        founded.attempts++;
        
        cout << founded.attempts << "%: Пароль - " << tpassword << endl;

        found = extractArchive(archivePath, outputPath, tpassword);
        if (found == 0) {
            founded.password = tpassword;
            return founded;
        } else if (found == 2560) {
            cout << "Неверное указан путь к файлу или место разархивирования" << endl;
            exit(1);
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
    returns datas;

    cout << "Начало поиска пароля:" << endl;
    
    auto start = high_resolution_clock::now();

    datas = FindPassword(archivePath, outputPath);

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);

    if (datas.password != "") {
        cout << "Найденный пароль: " << datas.password << endl;
        cout << "Количество попыток: " << datas.attempts << endl;
        cout << "Затрачено: " << duration.count() << " ms" << endl;
    } else {
        cout << "Ошибка: Пароль не найден (проверьте ввод)." << endl;
    }
    
    return 0;
}





/*
bool extractArchive(const string& archivePath, const string& outputPath, const string& password) {
    string command = "unrar x -p" + password + " \"" + archivePath + "\" \"" + outputPath + "\"";

    // Выполняем команду
    int result = system(command.c_str());

    // Если результат выполнения команды не равен 0, значит, что-то пошло не так (неверный пароль и т.д.)
    return (result == 0);
}
*/


/*
    while (success != 0) {
        password = getPassword();
        success = extractArchive(archivePath, outputPath, password);
        
        if (!success) {
            cout << "Пароль неверный, попробуйте снова." << endl;
        } else {
            cout << "Архив успешно разархивирован!" << endl;
        }
    }
*/

