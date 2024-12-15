#include <iostream>
#include <string>
#include <cstdlib>
#include <chrono>

using namespace std;
using namespace chrono;

int nowTry= 0;
int tryes = 10000;

typedef struct returns {
    string password;
    long long attempts;
    int found;
}returns;

int extractArchive(const string& archivePath, const string& outputPath, const string& password) {
    if (password != "abcd") {
        return 1;
    }
    return 0;
}

void printProgressBar (int attemp, const int combinations) {
    int temp = combinations / tryes;
    int ntf = nowTry * temp;
    int percent = (attemp - ntf)  * 100 /  temp;
    int pos = 50; //один символ 2%
    int filled = percent * pos / 100; 
    //printf("attemp: %d, combinations: %d temp: %d, ntf: %d, percent: %d, filled: %d\n", attemp, combinations, temp, ntf, percent, filled);
    cout << "\rПрогресс: ["; // Выводим начало прогресс-бара \r
    //вывод заполнености текущей стадии
    for (int i = 0; i < pos; i++) {
        if (i < filled) {
            cout << '%';
        } else {
            cout << '.'; 
        }
    }
    //конец вывода
    cout << "] ";// << nowTry + 1 << " of " << tryes; // Выводим начало прогресс-бара
    cout.flush(); // Сбрасываем буфер, чтобы увидеть изменения немедленно
    nowTry = attemp/temp;
    //this_thread::sleep_for(std::chrono::milliseconds(10));
}

returns FindPassword(const string& archivePath, const string& outputPath) {

    const string chars = "abcdefghijklmnopqrstuvwxyz0123456789"; //37 - 1874161
    returns founded = {"", 0, 1};

    const int len = chars.length(); //длина символов
    const int combinations = len*len*len*len; //кол-во комбинаций

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

                    //cout << founded.attempts << "%: Пароль - " << tpassword << endl; //вывод пароля
                    printProgressBar(founded.attempts, combinations);
                    founded.found = extractArchive(archivePath, outputPath, tpassword);
                    if (founded.found == 0) {
                        founded.password = tpassword;
                        //костыль в твоём коде потому что я не могу понять как потереть всю строку
                        cout << "\r\033[KПрогресс: 100% пароль найден" << endl;
                        //cout << endl << "Прогресс: 100% пароль найден" << endl; //если не нужно чистить прогрессбар
                        //cout.flush();  
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
    string archivePath = "getArchivePath()";

    // получаем путь куда разархивируем
    string outputPath = "getOutputPath()";

    // данные
    //returns datas;
    returns datas = {"", 0, 1};

    cout << "Начало поиска пароля:" << endl;

    auto start = high_resolution_clock::now();

    datas = FindPassword(archivePath, outputPath);
    
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