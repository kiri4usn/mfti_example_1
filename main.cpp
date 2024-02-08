#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <stdexcept>
#include <math.h>

using namespace std;

/*******************************
/  Структура SKU(уникальная единица товара)
******************************/

struct SKU{
    string SKU_name; // Имя уникальной единицы товара
    float SKU_price; // Цена уникальной единицы товара
    float SKU_promotion_price;
    float SKU_stock; // Остаток на складе
    int SKU_type; // 1 - весовой; 0 - штучный
};

/*******************************
/  Оформление(цвет текста, фона) init 0 сбрасывает стили
******************************/

string set_color(string text_color, string bg_color, bool init){
    if(init == 1){
        return (text_color == ""?"\x1b["+bg_color+"m":bg_color == ""?"\x1b["+text_color+"m":"\x1b["+text_color+";"+bg_color+"m");
    } else if(init == 0){
        return "\x1b[0m";
    }
}

/*******************************
/ Вывод склада/корзины flag = 1 - склад(по умолчанию); flag = 0 - корзина
******************************/

void print_struct(vector<SKU> stock, bool flag = 1) {
    for(int i = 0; i < stock.size(); i++){
        cout
                << "\x1b[1m"
                << "id:" << i << " " << stock[i].SKU_name << ((int)stock[i].SKU_promotion_price!=0?" (Акция)":"")
                << (flag == 1? set_color("","", 0):"")
                << "\n"
                << "Цена: "
                << setw(10)
                << left
                << (stock[i].SKU_promotion_price!=0?stock[i].SKU_promotion_price:stock[i].SKU_price)
                << (flag == 1?"  Остаток на складе: ":"  *  ")
                << stock[i].SKU_stock
                << (stock[i].SKU_type==1?" Кг":" Шт")
                <<"\n\n";
    }
};

/*******************************
/ Сумма пакупки
******************************/

float cart_summ(vector<SKU> cart){
    float Cart_summ = 0;
    for(int i = 0; i < cart.size(); i++){
        Cart_summ += (cart[i].SKU_promotion_price!=0?cart[i].SKU_promotion_price:cart[i].SKU_price) * cart[i].SKU_stock;
    }
    return Cart_summ;
}

/*******************************
/ Делаем round() удобнее
******************************/

float round2(float num, float precision)
{
    float num2; //Добавил для блок-схемы
    num2= round(num / precision) * precision;
    return num2;
}

/*******************************
/ Для особо настойчивых, кто любит вместо 1 или 0 ввести что-то другое
******************************/

string bool_check(string num) {
    if (num == "0" || num == "1") {
        return num;
    } else {
        cout << "\x1b[1;4m" << set_color("41", "30", 1) << "Только 1 - да, 0 - нет: " << set_color("", "", 0);
        cin >> num;
        return bool_check(num);
    }
}

/*******************************
/ Для особо настойчивых, кто любит вместо int ввести что-то другое
******************************/

string int_check(string num){
    try {
        if(stof(num) - stoi(num) == 0) {
            return num;
        } else {
            return int_check("num");
        }
    } catch (invalid_argument e){
        cout << "\x1b[1;4m" << set_color("41", "30", 1) << "Вводить можно только целое число: " << set_color("", "", 0);
        cin >> num;
        return int_check(num);
    }
}

/*******************************
/ Для особо настойчивых, кто любит вместо float ввести что-то другое
******************************/

string float_check(string num){
    try {
        return to_string(stof(num));
    } catch (invalid_argument e){
        cout << "\x1b[1;4m" << set_color("41", "30", 1) << "Вводить можно только число: " << set_color("", "", 0);
        cin >> num;
        return float_check(num);
    }
}

/*******************************
/ Проверяем введенное колличество. Если товар весовой то ок, а вот если нет...
******************************/

string count_check(string count, bool type){
    if(type == 0 && stof(count) == round2(stof(count),1)){
        return count;
    } else if (type == 0){
        cout << "\x1b[1;4m" << set_color("41", "30", 1) << "Товар не весовой, введите целое число: " << set_color("", "", 0);
        cin >> count;
        return count_check(float_check(count), type);
    } else {
        return float_check(count);
    }
}

int main(){

    /*******************************
    / настройки и бъявление переменных
    ******************************/

    setlocale(LC_ALL,".1251"); // Локализация (SetConsoleCP и SetConsoleOutputCP не подходят -> зависимы от windows.h)
    SKU stock_struct;
    vector<SKU> stock; // Вектор структур склада
    vector<SKU> cart; // Вектор структур корзины
    bool flag2 = 0; // Свободный флаг (для перебора корзины и/или диалога)

    string S_id; // для диалога
    string S_flag = "1"; // для диалога
    string S_count; // для диалога

    int loyalty_bonuses = 570; // 1 рубль = 10 бонусов
    int loyalty_discount = 5; // скидка 5%
    string summ;
    string U_name;

    /*******************************
    / Добавляем базовые SKU
    ******************************/

    stock_struct.SKU_name = "Селедка";
    stock_struct.SKU_price = 54.99;
    stock_struct.SKU_stock = 14.92;
    stock_struct.SKU_promotion_price = 0;
    stock_struct.SKU_type = 1; // Весовой товар
    stock.push_back(stock_struct);

    stock_struct.SKU_name = "Молоко";
    stock_struct.SKU_price = 129.99;
    stock_struct.SKU_stock = 2.0;
    stock_struct.SKU_promotion_price = 99.99; // !Добавили акцию!
    stock_struct.SKU_type = 0; // Штучный товар
    stock.push_back(stock_struct);

    stock_struct.SKU_name = "Соленые огурцы";
    stock_struct.SKU_price = 129.99;
    stock_struct.SKU_stock = 4.12;
    stock_struct.SKU_promotion_price = 0;
    stock_struct.SKU_type = 1; // Весовой товар
    stock.push_back(stock_struct);

    /*******************************
    / Название
    ******************************/
    cout << "Как к вам обращаться? ";
    cin >> U_name;
    cout << set_color("41","30",1)
         << "************************************************\n"
         << "                 Шестерочка\n"
         << "************************************************\n"
         << set_color("","",0)
         << "\n\nПриветствуем, " << U_name << "\n\n"
         << "\nСписок доступных продуктов:\n\n";

    print_struct(stock); // Доступные товары

    /*******************************
    / "Процедура покупки"
    ******************************/

    while (stoi(S_flag)){
        S_flag = true;
        cout << "\nВведите id товара: ";
        cin >> S_id;
        S_id = int_check(S_id); // проверка (точно int?)

        if(stoi(S_id) <= stock.size()-1) {
            cout << "Введите колличество: ";
            cin >> S_count;
            S_count = float_check(S_count); // проверка (точно float?)
            S_count = count_check(S_count,stock[stoi(S_id)].SKU_type); // проверка (вес с типом товара совпадает?)
            if (stock[stoi(S_id)].SKU_stock >= stof(S_count)) { // Сравниваем колличество желаемого товара с остатком на складе
                for (int i = 0; i < cart.size(); i++) { // перебор корзины (есть ли этот в ней уже)
                    if (stock[stoi(S_id)].SKU_name == cart[i].SKU_name) {
                        flag2 = true;
                    }
                }
                if (!flag2) {
                    cart.push_back(stock[stoi(S_id)]);
                    cart[cart.size() - 1].SKU_stock = stof(S_count);
                } else {
                    cart[cart.size() - 1].SKU_stock += stof(S_count);
                }
                stock[stoi(S_id)].SKU_stock -= stof(S_count); // Вычитаем товар со склада
                cout << "Вы добавили товар в корзину\n";
            } else {
                cout << "У нас столько нет, приходите позже(((\n";
            }
        } else {
            cout << "id не существует, но скоро завезут(((\n";
        }
        cout << "Продолжить пакупки? 1 - да, 0 - нет: ";
        cin >> S_flag;
        S_flag = bool_check(S_flag);
    }
    cout << "У вас накопилось " << loyalty_bonuses << " бонусов.\n1 руб = 10 бонусов. Списать? 1 - да, 0 - нет ";
    cin >> S_flag;
    S_flag = bool_check(S_flag);

    cout << "\n\x1b[1;4m" << set_color ("30","47",1) << "Чек\n";

    print_struct(cart, 0);

    cout<< "________________________________\n"
        << "Сумма до скидки: " << round2(cart_summ(cart), 0.01)
        << "\nБонусов списанно: " << (stoi(S_flag)?loyalty_bonuses/10:0) << " руб"
        << "\nCкидка: " << loyalty_discount << "%\n"
        << "Итого: " << round2(cart_summ(cart)*(1.0-loyalty_discount/100.0) - (stoi(S_flag)?loyalty_bonuses/10:0), 0.01) << "\n"
        << set_color("","",0) << "\nВведите наличные: ";
    cin >> summ;
    cout << "Сдача: " << stoi(int_check(summ)) - round2(cart_summ(cart)*(1.0-loyalty_discount/100.0) - (stoi(S_flag)?loyalty_bonuses/10:0), 0.01);

    return 0;
}
