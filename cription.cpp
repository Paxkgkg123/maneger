#include <string>

using namespace std;

string encryption(string password, string master_pass) { // шифр виженера на максималках
    string cur;
    char c;
    int n, t;
    for (int i = 0; i < password.size(); i++) {
        n = i % master_pass.size(); // номер символа сдвига
        t = ((int) password[i] + (int) master_pass[n] - 48); // сдвинутый символ
        if (t > 122)
            c = (char) (t - 74); // с = [48;122]
        cur.insert(0, 1, (char) c); // записывает полученные символы в начало
    } // шифр виженера + зеркальный переворот полученной записи
    password.erase();
    for (int i = 0; i < cur.size(); i++) {
        n = i % master_pass.size(); // номер символа сдвига
        t = ((int) cur[i] + (int) master_pass[n] - 48); // сдвинутый символ
        if (t > 122)
            c = (char) (t - 74); // с = [48;122]
        password.push_back((char) c); // записывает полученные символы
    } // чисто шифр виженера
    return password;
}
string decryption(string password, string master_pass) { // шифр виженера на максималках
    string cur;
    char c;
    int n, t;
    for (int i = 0; i < password.size(); i++) {
        n = i % master_pass.size(); // номер символа сдвига
        t = ((int) password[i] - (int) master_pass[n] + 48); // сдвинутый символ
        if (t < 48)
            c = (char) (t + 74); // с = [48;122]
        cur.insert(0, 1, (char) c); // записывает полученные символы в начало
    } // шифр виженера + зеркальный переворот полученной записи
    password.erase();
    for (int i = 0; i < cur.size(); i++) {
        n = i % master_pass.size(); // номер символа сдвига
        t = ((int) cur[i] - (int) master_pass[n] + 48); // сдвинутый символ
        if (t < 48)
            c = (char) (t + 74); // с = [48;122]
        password.push_back((char) c); // записывает полученные символы
    } // чисто шифр виженера
    return password;
}