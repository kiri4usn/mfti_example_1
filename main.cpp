#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

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
/  Оформление(цвет текста, фона)
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
            << "id:" << i << "  \x1b[1;4m" << stock[i].SKU_name << ((int)stock[i].SKU_promotion_price!=0?" (Акция)":"")
            << (flag == 1?"\x1b[0m":"")
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

double check_round(double value, double precision)
{
    return round(value / precision) * precision;
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

    int S_id; // для диалога
    bool S_flag = 1; // для диалога
    float S_count; // для диалога

    int loyalty_bonuses = 570; // 1 рубль = 10 бонусов
    int loyalty_discount = 5; // скидка 5%


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

    cout << set_color("41","30",1)
    << "************************************************\n"
    << "                 Шестерочка\n"
    << "************************************************\n"
    << set_color("","",0)
    << "\nСписок доступных продуктов:\n\n";

    print_struct(stock); // Доступные товары


    /*******************************
    / "Процедура покупки"
    ******************************/

    while (S_flag){
        cout << "\nВведите id товара: ";
        cin >> S_id;
        if(S_id <= stock.size()-1) {
            cout << "Введите колличество: ";
            cin >> S_count;
            if (stock[S_id].SKU_stock >= S_count) { // Сравниваем колличество желаемого товара с остатком на складе
                for (int i = 0; i < cart.size(); i++) { // перебор корзины (есть ли этот в ней уже)
                    if (stock[S_id].SKU_name == cart[i].SKU_name) {
                        flag2 = 1;
                    }
                }
                if (!flag2) {
                    cart.push_back(stock[S_id]);
                    cart[cart.size() - 1].SKU_stock = S_count;
                } else {
                    cart[cart.size() - 1].SKU_stock += S_count;
                }

                stock[S_id].SKU_stock -= S_count; // Вычитаем товар со склада
                cout << "Вы добавили товар в корзину\n";
            } else {
                cout << "У нас столько нет, приходите позже(((\n";
            }
        } else {
            cout << "id не существует, но скоро завезут(((\n";
        }

        cout << "Продолжить пакупки? 1 - да, 0 - нет: ";

        cin >> S_flag; // Только 0 или 1 иначе уйдет в бесконечный цикл
    }

    cout << "У вас накопилось " << loyalty_bonuses << " бонусов. Списать? 1 - да, 0 - нет ";
    cin >> flag2;
    cout << "\n\x1b[1;4m" << set_color ("30","47",1) << "Чек\n";

    print_struct(cart, 0);

    cout<< "________________________________\n"
            "Сумма до скидки: " << check_round(cart_summ(cart), 0.01) <<
            "\nБонусов списанно: " << (flag2?loyalty_bonuses/10:0) << " руб"
            "\nCкидка: " << loyalty_discount << "%\n"
            << "Итого: " << check_round(cart_summ(cart)*(1.0-loyalty_discount/100.0) - (flag2?loyalty_bonuses/10:0), 0.01) << "\n\n";

    return 0;
}
