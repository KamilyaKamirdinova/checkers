#include <iostream>
//модули библиотеки sfml 
#include <SFML/Graphics.hpp> //2D графический модуль: спрайты, текст, фигуры, ..
#include <SFML/Audio.hpp>//Звуки, потоковая передача
#include <SFML/System.hpp>//Базовый модуль SFML, определяющий различные утилиты
#include <SFML/Window.hpp>//Предоставляет окна на основе OpenGL и абстракции для обработки событий и ввода
#include <SFML/Network.hpp>//Связь на основе сокетов, утилиты и сетевые протоколы более высокого уровня (HTTP, FTP)
//i - вертикаль, j - горизонталь
using namespace std;
using namespace sf;
class Board {//класс доски
public:
    int size = 8;//размер доски
    //sf - пространство имён (по типу std::) sfml
    void Draw(sf::RenderWindow& window) {//создание окна

        sf::RectangleShape tile;//  создаём прямоугольный объект tile (1 квадратное поле доски)
        tile.setSize(sf::Vector2f(75.f, 75.f));// задаём его размер, (да, он задаётся через вектор-проекции ширины и длины, в нашем случае флоутовский)
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                tile.setPosition(sf::Vector2f(75.f * i, 75.f * j));//даём позицию каждой клеточки поля
                if ((i + j) % 2 == 0) {
                    tile.setFillColor(sf::Color::White);//закрашиваем белые поля, начало - верхни левый угол
                }
                else {
                    tile.setFillColor(sf::Color(100, 70, 36));//цвет коричневый 
                }
                window.draw(tile);
            }
        }
    }

    void Highlight(sf::RenderWindow& window, int  x, int y) {//окошко подсвечивания принажатии, создаётся новое
        sf::RectangleShape rect;
        rect.setSize(sf::Vector2f(75.f, 75.f));
        rect.setFillColor(sf::Color(140, 255, 50));
        rect.setPosition(sf::Vector2f(75.f * x, 75.f * y));//светло-зелёный
        window.draw(rect);
    }
};
//______________
class Piece {
public:
    float radius = 30;//значение радиуса шашки
    int x = 0;
    int y = 0;
    bool isAlive = false;//обычная шашка
    bool isKing = false;//дамка

    sf::Color color;// переменная color типа color

    void Draw(sf::RenderWindow& window) {
        if (isAlive) {
            sf::CircleShape shape(radius);//задаём радиус
            shape.setFillColor(color);//конструктор, цвет задаётся в main
            if (isKing) {
                shape.setOutlineThickness(3.f); //толщина ободочка
                shape.setOutlineColor(sf::Color::Yellow);//цвет ободочка
            }
            shape.setPosition(sf::Vector2f(x * 75 + (75 - radius * 2) / 2, y * 75 + (75 - 2 * radius) / 2));//шашки по позициям

            window.draw(shape);//создаётся окно шашка
        }
    }
};
//______________
void Setup(sf::RenderWindow& window, Piece* BlackPieces, Piece* WhitePieces) {//продолжается позиционирование шашек. Даются значения x,y. Через i,j;
    int m = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = (int)(i % 2 == 0); j < 8; j += 2) {//
            WhitePieces[m].isAlive = true;//позиционирование белых шашек
            WhitePieces[m].x = j;
            WhitePieces[m].y = i;
            m++;
        }

    }
    m = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = (int)(i % 2 == 1); j < 8; j += 2) {//
            BlackPieces[m].isAlive = true;//чёрных шашек
            BlackPieces[m].x = j;
            BlackPieces[m].y = 7 - i;//снизу
            m++;
        }

    }
}
//______________
Piece* FindPiece(int x, int y, Piece* BlackPieces, Piece* WhitePieces) {//нахождение местоположения шашки
    for (int i = 0; i < 12; i++) {
        if (BlackPieces[i].x == x && BlackPieces[i].y == y) {
            if (BlackPieces[i].isAlive) {
                return &BlackPieces[i];
            }
        }
        if (WhitePieces[i].x == x && WhitePieces[i].y == y) {
            if (WhitePieces[i].isAlive) {
                return &WhitePieces[i];
            }
        }
    }
    return NULL;
}
//______________
void Win(int x, int y, Piece* BlackPieces, Piece* WhitePieces, int* turn) {
    int k = 0;//Счётчик для белых шашек
    int n = 0;//счётчик для черных шашек
    int t = 0;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            x = i;
            y = j;
            if (FindPiece(x, y, BlackPieces, WhitePieces) != NULL && FindPiece(x, y, BlackPieces, WhitePieces)->color == sf::Color::White) {
                k++;
            }
            if (FindPiece(x, y, BlackPieces, WhitePieces) != NULL && FindPiece(x, y, BlackPieces, WhitePieces)->color == sf::Color::Black) {
                n++;
            }
            if (!FindPiece(x, y, BlackPieces, WhitePieces)) {
                t++;
            }
        }
    }
    using namespace sf;
    if (k == 0) {
        RenderWindow window(sf::VideoMode(612, 408), "", sf::Style::Default);
        window.setFramerateLimit(60);

        Texture texture;
        if (!texture.loadFromFile("istockphoto-1186104651-612x612.jpg"))
        {
            std::cout << "Could not load enemy texture";
            system("pause");
        }
        Sprite sprite;
        sprite.setTexture(texture);

        sprite.setPosition(0, 0);
        sf::Font font;
        if (!font.loadFromFile("Revue Rus Italic.ttf"))
        {
            std::cout << "Error " << std::endl;
            system("pause");
        }
        sf::Text text;
        text.setFont(font);
        text.setCharacterSize(25);
        text.setFillColor(sf::Color::White);
        text.setStyle(sf::Text::Style::Bold);
        text.setPosition(170.f, window.getSize().y / 3.f);


        text.setString("Победа черных!");
        while (window.isOpen())
        {
            // window.draw(winner);
             // проверяем все события окна,которые были запущены после последней итерации цикла
            sf::Event event;
            while (window.pollEvent(event))
            {
                // если произошло событие Закрытие,закрываем наше окно
                if (event.type == sf::Event::Closed)
                    window.close();
            }
            window.clear();
            window.draw(sprite);
            window.draw(text);
            window.display();
        }
    }
    if (n == 0) {
        RenderWindow window(sf::VideoMode(612, 408), "", sf::Style::Default);
        window.setFramerateLimit(60);
        Texture texture;
        if (!texture.loadFromFile("istockphoto-1186104651-612x612.jpg"))
        {
            std::cout << "Could not load enemy texture";

        }
        Sprite sprite;
        sprite.setTexture(texture);

        sprite.setPosition(0, 0);
        Font font;
        if (!font.loadFromFile("Revue Rus Italic.ttf"))
        {
            std::cout << "Error " << std::endl;
            system("pause");
        }
        sf::Text text;
        text.setFont(font);
        text.setCharacterSize(25);
        text.setFillColor(sf::Color::White);
        text.setStyle(sf::Text::Style::Bold);
        text.setPosition(170.f, window.getSize().y / 3.f);

        text.setString("Победа белых!");
        while (window.isOpen())
        {
            // window.draw(winner);
             // проверяем все события окна,которые были запущены после последней итерации цикла
            sf::Event event;
            while (window.pollEvent(event))
            {
                // если произошло событие Закрытие,закрываем наше окно
                if (event.type == sf::Event::Closed)
                    window.close();
            }
            window.clear();
            window.draw(sprite);
            window.draw(text);
            window.display();
        }
    }
}
//______________
void KillPiece(int x, int y, Piece* BlackPieces, Piece* WhitePieces, int* turn) {//ПОЕДАНИЕ АХАХА!
    FindPiece(x, y, BlackPieces, WhitePieces)->isAlive = false;
    //*turn = ((*turn == 1) ? 2 : 1);//turn - очередь хода.  Если съел - делай ход ещё
    return;
}
//______________
int ObyazHodCep(int x, int y, Piece* s_Piece, Piece* BlackPieces, Piece* WhitePieces, int* turn)//Белые
{
    int n = 0;//счётчик белых 
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            y = i;
            x = j;
            if (FindPiece(x, y, BlackPieces, WhitePieces) != NULL && FindPiece(x, y, BlackPieces, WhitePieces)->color == sf::Color::White) {

                if (FindPiece(x - 1, y - 1, BlackPieces, WhitePieces) != NULL && FindPiece(x - 1, y - 1, BlackPieces, WhitePieces)->color == sf::Color::Black) {
                    if (!FindPiece(x - 2, y - 2, BlackPieces, WhitePieces)) {
                        if (y - 2 < 0 || x - 2 < 0) {
                            continue;
                        }
                        n++;
                    }
                }

                if (FindPiece(x - 1, y + 1, BlackPieces, WhitePieces) != NULL && FindPiece(x - 1, y + 1, BlackPieces, WhitePieces)->color == sf::Color::Black) {
                    if (!FindPiece(x - 2, y + 2, BlackPieces, WhitePieces)) {
                        if (x - 2 < 0 || y + 2 > 7) {
                            continue;
                        }
                        n++;
                    }
                }

                if (FindPiece(x + 1, y + 1, BlackPieces, WhitePieces) != NULL && FindPiece(x + 1, y + 1, BlackPieces, WhitePieces)->color == sf::Color::Black) {
                    if (!FindPiece(x + 2, y + 2, BlackPieces, WhitePieces)) {
                        if (x + 2 > 7 || y + 2 > 7) {
                            continue;
                        }
                        n++;
                    }
                }

                if (FindPiece(x + 1, y - 1, BlackPieces, WhitePieces) != NULL && FindPiece(x + 1, y - 1, BlackPieces, WhitePieces)->color == sf::Color::Black) {
                    if (!FindPiece(x + 2, y - 2, BlackPieces, WhitePieces)) {
                        if (x + 2 > 7 || y - 2 < 0) {
                            continue;
                        }
                        n++;
                    }
                }
            }


        }
    }
    if (n == 0) {
        return -1;
    }
    else
    {

        return 0;

    }



}
//______________
int ObyazHod2Cep(int x, int y, Piece* s_Piece, Piece* BlackPieces, Piece* WhitePieces, int* turn) {
    int k = 0;//счётчик чёрных
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            y = i;
            x = j;
            if (FindPiece(x, y, BlackPieces, WhitePieces) != NULL && FindPiece(x, y, BlackPieces, WhitePieces)->color == sf::Color::Black) {

                if (FindPiece(x - 1, y - 1, BlackPieces, WhitePieces) != NULL && FindPiece(x - 1, y - 1, BlackPieces, WhitePieces)->color == sf::Color::White) {
                    if (!FindPiece(x - 2, y - 2, BlackPieces, WhitePieces)) {
                        if (y - 2 < 0 || x - 2 < 0) {
                            continue;
                        }
                        k++;
                    }
                }

                if (FindPiece(x - 1, y + 1, BlackPieces, WhitePieces) != NULL && FindPiece(x - 1, y + 1, BlackPieces, WhitePieces)->color == sf::Color::White) {
                    if (!FindPiece(x - 2, y + 2, BlackPieces, WhitePieces)) {
                        if (x - 2 < 0 || y + 2 > 7) {
                            continue;
                        }
                        k++;
                    }
                }

                if (FindPiece(x + 1, y + 1, BlackPieces, WhitePieces) != NULL && FindPiece(x + 1, y + 1, BlackPieces, WhitePieces)->color == sf::Color::White) {
                    if (!FindPiece(x + 2, y + 2, BlackPieces, WhitePieces)) {
                        if (x + 2 > 7 || y + 2 > 7) {
                            continue;
                        }
                        k++;
                    }
                }

                if (FindPiece(x + 1, y - 1, BlackPieces, WhitePieces) != NULL && FindPiece(x + 1, y - 1, BlackPieces, WhitePieces)->color == sf::Color::White) {
                    if (!FindPiece(x + 2, y - 2, BlackPieces, WhitePieces)) {
                        if (x + 2 > 7 || y - 2 < 0) {
                            continue;
                        }
                        k++;
                    }
                }
            }

        }
    }
    if (k == 0) {
        return -2;
    }
    else
    {

        return 0;

    }

}
//______________
int ObyazHod(int x, int y, Piece* s_Piece, Piece* BlackPieces, Piece* WhitePieces, int* turn)//Белые
{
    int n = 0;//счётчик белых 
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            y = i;
            x = j;
            if (FindPiece(x, y, BlackPieces, WhitePieces) != NULL && FindPiece(x, y, BlackPieces, WhitePieces)->color == sf::Color::White) {

                if (FindPiece(x - 1, y - 1, BlackPieces, WhitePieces) != NULL && FindPiece(x - 1, y - 1, BlackPieces, WhitePieces)->color == sf::Color::Black) {
                    if (!FindPiece(x - 2, y - 2, BlackPieces, WhitePieces)) {
                        if (y - 2 < 0 || x - 2 < 0) {
                            continue;
                        }
                        n++;
                    }
                }

                if (FindPiece(x - 1, y + 1, BlackPieces, WhitePieces) != NULL && FindPiece(x - 1, y + 1, BlackPieces, WhitePieces)->color == sf::Color::Black) {
                    if (!FindPiece(x - 2, y + 2, BlackPieces, WhitePieces)) {
                        if (x - 2 < 0 || y + 2 > 7) {
                            continue;
                        }
                        n++;
                    }
                }

                if (FindPiece(x + 1, y + 1, BlackPieces, WhitePieces) != NULL && FindPiece(x + 1, y + 1, BlackPieces, WhitePieces)->color == sf::Color::Black) {
                    if (!FindPiece(x + 2, y + 2, BlackPieces, WhitePieces)) {
                        if (x + 2 > 7 || y + 2 > 7) {
                            continue;
                        }
                        n++;
                    }
                }

                if (FindPiece(x + 1, y - 1, BlackPieces, WhitePieces) != NULL && FindPiece(x + 1, y - 1, BlackPieces, WhitePieces)->color == sf::Color::Black) {
                    if (!FindPiece(x + 2, y - 2, BlackPieces, WhitePieces)) {
                        if (x + 2 > 7 || y - 2 < 0) {
                            continue;
                        }
                        n++;
                    }
                }
            }


        }
    }
    if (n == 0) {
        return -1;
    }
    else
    {

        RenderWindow window(sf::VideoMode(250, 250), "", sf::Style::Default);
        window.setFramerateLimit(60);

        Texture texture;
        if (!texture.loadFromFile("unnamed.jpg"))
        {
            std::cout << "Could not load enemy texture";
            system("pause");
        }
        Sprite sprite;
        sprite.setTexture(texture);

        sprite.setPosition(0, 0);
        sf::Font font;
        if (!font.loadFromFile("Revue Rus Italic.ttf"))
        {
            std::cout << "Error " << std::endl;
            system("pause");
        }
        sf::Vector2f targetSize(250.0f, 250.0f); //целевой размер

        sprite.setScale(
            targetSize.x / sprite.getLocalBounds().width,
            targetSize.y / sprite.getLocalBounds().height);
        sf::Text text;
        text.setFont(font);
        text.setCharacterSize(23);
        text.setFillColor(sf::Color::White);
        text.setStyle(sf::Text::Style::Bold);
        text.setPosition(12.f, window.getSize().y / 11.f);


        text.setString("Скушайте шашку!");
 

        while (window.isOpen())
        {
            // window.draw(winner);
             // проверяем все события окна,которые были запущены после последней итерации цикла
            sf::Event event;
            while (window.pollEvent(event))
            {
                // если произошло событие Закрытие,закрываем наше окно
                if (event.type == sf::Event::Closed)
                    window.close();
            }
            window.clear();
            window.draw(sprite);
            window.draw(text);
            window.display();
        }
    }

    }




//______________
int ObyazHod2(int x, int y, Piece* s_Piece, Piece* BlackPieces, Piece* WhitePieces, int* turn) {
    int k = 0;//счётчик чёрных
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            y = i;
            x = j;
            if (FindPiece(x, y, BlackPieces, WhitePieces) != NULL && FindPiece(x, y, BlackPieces, WhitePieces)->color == sf::Color::Black) {

                if (FindPiece(x - 1, y - 1, BlackPieces, WhitePieces) != NULL && FindPiece(x - 1, y - 1, BlackPieces, WhitePieces)->color == sf::Color::White) {
                    if (!FindPiece(x - 2, y - 2, BlackPieces, WhitePieces)) {
                        if (y - 2 < 0 || x - 2 < 0) {
                            continue;
                        }
                        k++;
                    }
                }

                if (FindPiece(x - 1, y + 1, BlackPieces, WhitePieces) != NULL && FindPiece(x - 1, y + 1, BlackPieces, WhitePieces)->color == sf::Color::White) {
                    if (!FindPiece(x - 2, y + 2, BlackPieces, WhitePieces)) {
                        if (x - 2 < 0 || y + 2 > 7) {
                            continue;
                        }
                        k++;
                    }
                }

                if (FindPiece(x + 1, y + 1, BlackPieces, WhitePieces) != NULL && FindPiece(x + 1, y + 1, BlackPieces, WhitePieces)->color == sf::Color::White) {
                    if (!FindPiece(x + 2, y + 2, BlackPieces, WhitePieces)) {
                        if (x + 2 > 7 || y + 2 > 7) {
                            continue;
                        }
                        k++;
                    }
                }

                if (FindPiece(x + 1, y - 1, BlackPieces, WhitePieces) != NULL && FindPiece(x + 1, y - 1, BlackPieces, WhitePieces)->color == sf::Color::White) {
                    if (!FindPiece(x + 2, y - 2, BlackPieces, WhitePieces)) {
                        if (x + 2 > 7 || y - 2 < 0) {
                            continue;
                        }
                        k++;
                    }
                }
            }

        }
    }
    if (k == 0) {
        return -2;
    }
    else
    {

        RenderWindow window(sf::VideoMode(250, 250), "", sf::Style::Default);
        window.setFramerateLimit(60);

        Texture texture;
        if (!texture.loadFromFile("unnamed.jpg"))
        {
            std::cout << "Could not load enemy texture";
            system("pause");
        }
        Sprite sprite;
        sprite.setTexture(texture);

        sprite.setPosition(0, 0);
        sf::Font font;
        if (!font.loadFromFile("Revue Rus Italic.ttf"))
        {
            std::cout << "Error " << std::endl;
            system("pause");
        }
        sf::Vector2f targetSize(250.0f, 250.0f); //целевой размер

        sprite.setScale(
            targetSize.x / sprite.getLocalBounds().width,
            targetSize.y / sprite.getLocalBounds().height);
        sf::Text text;
        text.setFont(font);
        text.setCharacterSize(23);
        text.setFillColor(sf::Color::White);
        text.setStyle(sf::Text::Style::Bold);
        text.setPosition(12.f, window.getSize().y / 11.f);


        text.setString("Скушайте шашку!");


        while (window.isOpen())
        {
            // window.draw(winner);
             // проверяем все события окна,которые были запущены после последней итерации цикла
            sf::Event event;
            while (window.pollEvent(event))
            {
                // если произошло событие Закрытие,закрываем наше окно
                if (event.type == sf::Event::Closed)
                    window.close();
            }
            window.clear();
            window.draw(sprite);
            window.draw(text);
            window.display();
        }
    }

}
//______________
int ObyazHodDama(int x, int y, Piece* s_Piece, Piece* BlackPieces, Piece* WhitePieces, int* turn) {
    int e = 0;//счётчик чёрных 
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            y = i;
            x = j;
            if (FindPiece(x, y, BlackPieces, WhitePieces) != NULL && FindPiece(x, y, BlackPieces, WhitePieces)->color == sf::Color::White && FindPiece(x, y, BlackPieces, WhitePieces)->isKing == true) {
                int n = 0;
                int l = 0;
                for (int i = 0; i < 8; i++) {
                    if (x + n > 7 || x - n < 0) {
                        break;
                    }
                    n++;

                    if (FindPiece(x + n, y - n, BlackPieces, WhitePieces) != NULL && FindPiece(x + n, y - n, BlackPieces, WhitePieces)->color == sf::Color::Black) {
                        break;
                    }
                }
                l = n + 1;
                if (x + l < 7 || y - l > 0)
                {
                    if (!FindPiece(x + l, y - l, BlackPieces, WhitePieces) && FindPiece(x + n, y - n, BlackPieces, WhitePieces) != NULL && FindPiece(x + n, y - n, BlackPieces, WhitePieces)->color == sf::Color::Black) {
                        e++;
                    }
                }
                n = 0;
                l = 0;
                for (int i = 0; i < 8; i++) {


                    if (x - n < 0 || y + n > 7) {
                        break;
                    }
                    n++;
                    if (FindPiece(x - n, y + n, BlackPieces, WhitePieces) != NULL && FindPiece(x - n, y + n, BlackPieces, WhitePieces)->color == sf::Color::Black) {
                        break;
                    }
                }
                l = n + 1;
                if (x - l > 0 || y + l < 7)
                {
                    if (!FindPiece(x - l, y + l, BlackPieces, WhitePieces) && FindPiece(x - n, y + n, BlackPieces, WhitePieces) != NULL && FindPiece(x - n, y + n, BlackPieces, WhitePieces)->color == sf::Color::Black) {
                        e++;
                    }
                }
                n = 0;
                l = 0;
                for (int i = 0; i < 8; i++) {
                    if (x + n > 7 || y + n > 0) {
                        break;
                    }
                    n++;
                    if (FindPiece(x + n, y + n, BlackPieces, WhitePieces) != NULL && FindPiece(x + n, y + n, BlackPieces, WhitePieces)->color == sf::Color::Black) {
                        break;
                    }
                }
                l = n + 1;
                if (x + l < 7 || y + l < 7)
                {
                    if (!FindPiece(x + l, y + l, BlackPieces, WhitePieces) && FindPiece(x + n, y + n, BlackPieces, WhitePieces) != NULL && FindPiece(x + n, y + n, BlackPieces, WhitePieces)->color == sf::Color::Black) {
                        e++;
                    }
                }
                n = 0;
                l = 0;
                for (int i = 0; i < 8; i++) {

                    if (x - n < 0 || y - n < 0) {
                        break;
                    }
                    n++;
                    if (FindPiece(x - n, y - n, BlackPieces, WhitePieces) != NULL && FindPiece(x - n, y - n, BlackPieces, WhitePieces)->color == sf::Color::Black) {
                        break;
                    }
                }
                l = n + 1;
                if (x - l > 0 || y - l > 0)
                {
                    if (!FindPiece(x - l, y - l, BlackPieces, WhitePieces) && FindPiece(x - n, y - n, BlackPieces, WhitePieces) != NULL && FindPiece(x - n, y - n, BlackPieces, WhitePieces)->color == sf::Color::Black) {
                        e++;
                    }

                }

            }


        }
    }
    if (e == 0) {
        return -3;
    }
    else
    {

        RenderWindow window(sf::VideoMode(250, 250), "", sf::Style::Default);
        window.setFramerateLimit(60);

        Texture texture;
        if (!texture.loadFromFile("unnamed.jpg"))
        {
            std::cout << "Could not load enemy texture";
            system("pause");
        }
        Sprite sprite;
        sprite.setTexture(texture);

        sprite.setPosition(0, 0);
        sf::Font font;
        if (!font.loadFromFile("Revue Rus Italic.ttf"))
        {
            std::cout << "Error " << std::endl;
            system("pause");
        }
        sf::Vector2f targetSize(250.0f, 250.0f); //целевой размер

        sprite.setScale(
            targetSize.x / sprite.getLocalBounds().width,
            targetSize.y / sprite.getLocalBounds().height);
        sf::Text text;
        text.setFont(font);
        text.setCharacterSize(19);
        text.setFillColor(sf::Color::White);
        text.setStyle(sf::Text::Style::Bold);
        text.setPosition(23.f, window.getSize().y / 20.f);


        text.setString("Скушайте шашку");
        sf::Text text1;
        text1.setFont(font);
        text1.setCharacterSize(19);
        text1.setFillColor(sf::Color::White);
        text1.setStyle(sf::Text::Style::Bold);
        text1.setPosition(70.f, window.getSize().y / 7.f);
        text1.setString("дамкой!");

        while (window.isOpen())
        {
            // window.draw(winner);
             // проверяем все события окна,которые были запущены после последней итерации цикла
            sf::Event event;
            while (window.pollEvent(event))
            {
                // если произошло событие Закрытие,закрываем наше окно
                if (event.type == sf::Event::Closed)
                    window.close();
            }
            window.clear();
            window.draw(sprite);
            window.draw(text);
            window.draw(text1);
            window.display();
        }
    }
}
//______________
int ObyazHodDama2(int x, int y, Piece* s_Piece, Piece* BlackPieces, Piece* WhitePieces, int* turn) {
    int e = 0;//счётчик чёрных 
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            y = i;
            x = j;
            if (FindPiece(x, y, BlackPieces, WhitePieces) != NULL && FindPiece(x, y, BlackPieces, WhitePieces)->color == sf::Color::Black && FindPiece(x, y, BlackPieces, WhitePieces)->isKing == true) {
                int n = 0;
                int l = 0;
                for (int i = 0; i < 8; i++) {
                    if (x + n > 7 || x - n < 0) {
                        break;
                    }
                    n++;

                    if (FindPiece(x + n, y - n, BlackPieces, WhitePieces) != NULL && FindPiece(x + n, y - n, BlackPieces, WhitePieces)->color == sf::Color::White) {
                        break;
                    }
                }
                l = n + 1;
                if (x + l < 7 || y - l > 0)
                {
                    if (!FindPiece(x + l, y - l, BlackPieces, WhitePieces) && FindPiece(x + n, y - n, BlackPieces, WhitePieces) != NULL && FindPiece(x + n, y - n, BlackPieces, WhitePieces)->color == sf::Color::White) {
                        e++;
                    }
                }
                n = 0;
                l = 0;
                for (int i = 0; i < 8; i++) {


                    if (x - n < 0 || y + n > 7) {
                        break;
                    }
                    n++;
                    if (FindPiece(x - n, y + n, BlackPieces, WhitePieces) != NULL && FindPiece(x - n, y + n, BlackPieces, WhitePieces)->color == sf::Color::White) {
                        break;
                    }
                }
                l = n + 1;
                if (x - l > 0 || y + l < 7)
                {
                    if (!FindPiece(x - l, y + l, BlackPieces, WhitePieces) && FindPiece(x - n, y + n, BlackPieces, WhitePieces) != NULL && FindPiece(x - n, y + n, BlackPieces, WhitePieces)->color == sf::Color::White) {
                        e++;
                    }
                }
                n = 0;
                l = 0;
                for (int i = 0; i < 8; i++) {
                    if (x + n > 7 || y + n > 0) {
                        break;
                    }
                    n++;
                    if (FindPiece(x + n, y + n, BlackPieces, WhitePieces) != NULL && FindPiece(x + n, y + n, BlackPieces, WhitePieces)->color == sf::Color::White) {
                        break;
                    }
                }
                l = n + 1;
                if (x + l < 7 || y + l < 7)
                {
                    if (!FindPiece(x + l, y + l, BlackPieces, WhitePieces) && FindPiece(x + n, y + n, BlackPieces, WhitePieces) != NULL && FindPiece(x + n, y + n, BlackPieces, WhitePieces)->color == sf::Color::White) {
                        e++;
                    }
                }
                n = 0;
                l = 0;
                for (int i = 0; i < 8; i++) {

                    if (x - n < 0 || y - n < 0) {
                        break;
                    }
                    n++;
                    if (FindPiece(x - n, y - n, BlackPieces, WhitePieces) != NULL && FindPiece(x - n, y - n, BlackPieces, WhitePieces)->color == sf::Color::White) {
                        break;
                    }
                }
                l = n + 1;
                if (x - l > 0 || y - l > 0)
                {
                    if (!FindPiece(x - l, y - l, BlackPieces, WhitePieces) && FindPiece(x - n, y - n, BlackPieces, WhitePieces) != NULL && FindPiece(x - n, y - n, BlackPieces, WhitePieces)->color == sf::Color::White) {
                        e++;
                    }

                }

            }


        }
    }
    if (e == 0) {
        return -4;
    }
    else
    {

        RenderWindow window(sf::VideoMode(250, 250), "", sf::Style::Default);
        window.setFramerateLimit(60);

        Texture texture;
        if (!texture.loadFromFile("unnamed.jpg"))
        {
            std::cout << "Could not load enemy texture";
            system("pause");
        }
        Sprite sprite;
        sprite.setTexture(texture);

        sprite.setPosition(0, 0);
        sf::Font font;
        if (!font.loadFromFile("Revue Rus Italic.ttf"))
        {
            std::cout << "Error " << std::endl;
            system("pause");
        }
        sf::Vector2f targetSize(250.0f, 250.0f); //целевой размер

        sprite.setScale(
            targetSize.x / sprite.getLocalBounds().width,
            targetSize.y / sprite.getLocalBounds().height);
        sf::Text text;
        text.setFont(font);
        text.setCharacterSize(19);
        text.setFillColor(sf::Color::White);
        text.setStyle(sf::Text::Style::Bold);
        text.setPosition(23.f, window.getSize().y / 20.f);


        text.setString("Скушайте шашку");
        sf::Text text1;
        text1.setFont(font);
        text1.setCharacterSize(19);
        text1.setFillColor(sf::Color::White);
        text1.setStyle(sf::Text::Style::Bold);
        text1.setPosition(70.f, window.getSize().y / 7.f);
        text1.setString("дамкой!");

        while (window.isOpen())
        {
            // window.draw(winner);
             // проверяем все события окна,которые были запущены после последней итерации цикла
            sf::Event event;
            while (window.pollEvent(event))
            {
                // если произошло событие Закрытие,закрываем наше окно
                if (event.type == sf::Event::Closed)
                    window.close();
            }
            window.clear();
            window.draw(sprite);
            window.draw(text);
            window.draw(text1);
            window.display();
        }
    }
}
//______________
int ObyazHodDamaCep(int x, int y, Piece* s_Piece, Piece* BlackPieces, Piece* WhitePieces, int* turn) {
    int e = 0;//счётчик чёрных 
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            y = i;
            x = j;
            if (FindPiece(x, y, BlackPieces, WhitePieces) != NULL && FindPiece(x, y, BlackPieces, WhitePieces)->color == sf::Color::White && FindPiece(x, y, BlackPieces, WhitePieces)->isKing == true) {
                int n = 0;
                int l = 0;
                for (int i = 0; i < 8; i++) {
                    if (x + n > 7 || x - n < 0) {
                        break;
                    }
                    n++;

                    if (FindPiece(x + n, y - n, BlackPieces, WhitePieces) != NULL && FindPiece(x + n, y - n, BlackPieces, WhitePieces)->color == sf::Color::Black) {
                        break;
                    }
                }
                l = n + 1;
                if (x + l < 7 || y - l > 0)
                {
                    if (!FindPiece(x + l, y - l, BlackPieces, WhitePieces) && FindPiece(x + n, y - n, BlackPieces, WhitePieces) != NULL && FindPiece(x + n, y - n, BlackPieces, WhitePieces)->color == sf::Color::Black) {
                        e++;
                    }
                }
                n = 0;
                l = 0;
                for (int i = 0; i < 8; i++) {


                    if (x - n < 0 || y + n > 7) {
                        break;
                    }
                    n++;
                    if (FindPiece(x - n, y + n, BlackPieces, WhitePieces) != NULL && FindPiece(x - n, y + n, BlackPieces, WhitePieces)->color == sf::Color::Black) {
                        break;
                    }
                }
                l = n + 1;
                if (x - l > 0 || y + l < 7)
                {
                    if (!FindPiece(x - l, y + l, BlackPieces, WhitePieces) && FindPiece(x - n, y + n, BlackPieces, WhitePieces) != NULL && FindPiece(x - n, y + n, BlackPieces, WhitePieces)->color == sf::Color::Black) {
                        e++;
                    }
                }
                n = 0;
                l = 0;
                for (int i = 0; i < 8; i++) {
                    if (x + n > 7 || y + n > 0) {
                        break;
                    }
                    n++;
                    if (FindPiece(x + n, y + n, BlackPieces, WhitePieces) != NULL && FindPiece(x + n, y + n, BlackPieces, WhitePieces)->color == sf::Color::Black) {
                        break;
                    }
                }
                l = n + 1;
                if (x + l < 7 || y + l < 7)
                {
                    if (!FindPiece(x + l, y + l, BlackPieces, WhitePieces) && FindPiece(x + n, y + n, BlackPieces, WhitePieces) != NULL && FindPiece(x + n, y + n, BlackPieces, WhitePieces)->color == sf::Color::Black) {
                        e++;
                    }
                }
                n = 0;
                l = 0;
                for (int i = 0; i < 8; i++) {

                    if (x - n < 0 || y - n < 0) {
                        break;
                    }
                    n++;
                    if (FindPiece(x - n, y - n, BlackPieces, WhitePieces) != NULL && FindPiece(x - n, y - n, BlackPieces, WhitePieces)->color == sf::Color::Black) {
                        break;
                    }
                }
                l = n + 1;
                if (x - l > 0 || y - l > 0)
                {
                    if (!FindPiece(x - l, y - l, BlackPieces, WhitePieces) && FindPiece(x - n, y - n, BlackPieces, WhitePieces) != NULL && FindPiece(x - n, y - n, BlackPieces, WhitePieces)->color == sf::Color::Black) {
                        e++;
                    }

                }

            }


        }
    }
    if (e == 0) {
        return -3;
    }
    else
    {

        return 0;
    }
}
//______________
int ObyazHodDama2Cep(int x, int y, Piece* s_Piece, Piece* BlackPieces, Piece* WhitePieces, int* turn) {
    int e = 0;//счётчик чёрных 
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            y = i;
            x = j;
            if (FindPiece(x, y, BlackPieces, WhitePieces) != NULL && FindPiece(x, y, BlackPieces, WhitePieces)->color == sf::Color::Black && FindPiece(x, y, BlackPieces, WhitePieces)->isKing == true) {
                int n = 0;
                int l = 0;
                for (int i = 0; i < 8; i++) {
                    if (x + n > 7 || x - n < 0) {
                        break;
                    }
                    n++;

                    if (FindPiece(x + n, y - n, BlackPieces, WhitePieces) != NULL && FindPiece(x + n, y - n, BlackPieces, WhitePieces)->color == sf::Color::White) {
                        break;
                    }
                }
                l = n + 1;
                if (x + l < 7 || y - l > 0)
                {
                    if (!FindPiece(x + l, y - l, BlackPieces, WhitePieces) && FindPiece(x + n, y - n, BlackPieces, WhitePieces) != NULL && FindPiece(x + n, y - n, BlackPieces, WhitePieces)->color == sf::Color::White) {
                        e++;
                    }
                }
                n = 0;
                l = 0;
                for (int i = 0; i < 8; i++) {


                    if (x - n < 0 || y + n > 7) {
                        break;
                    }
                    n++;
                    if (FindPiece(x - n, y + n, BlackPieces, WhitePieces) != NULL && FindPiece(x - n, y + n, BlackPieces, WhitePieces)->color == sf::Color::White) {
                        break;
                    }
                }
                l = n + 1;
                if (x - l > 0 || y + l < 7)
                {
                    if (!FindPiece(x - l, y + l, BlackPieces, WhitePieces) && FindPiece(x - n, y + n, BlackPieces, WhitePieces) != NULL && FindPiece(x - n, y + n, BlackPieces, WhitePieces)->color == sf::Color::White) {
                        e++;
                    }
                }
                n = 0;
                l = 0;
                for (int i = 0; i < 8; i++) {
                    if (x + n > 7 || y + n > 0) {
                        break;
                    }
                    n++;
                    if (FindPiece(x + n, y + n, BlackPieces, WhitePieces) != NULL && FindPiece(x + n, y + n, BlackPieces, WhitePieces)->color == sf::Color::White) {
                        break;
                    }
                }
                l = n + 1;
                if (x + l < 7 || y + l < 7)
                {
                    if (!FindPiece(x + l, y + l, BlackPieces, WhitePieces) && FindPiece(x + n, y + n, BlackPieces, WhitePieces) != NULL && FindPiece(x + n, y + n, BlackPieces, WhitePieces)->color == sf::Color::White) {
                        e++;
                    }
                }
                n = 0;
                l = 0;
                for (int i = 0; i < 8; i++) {

                    if (x - n < 0 || y - n < 0) {
                        break;
                    }
                    n++;
                    if (FindPiece(x - n, y - n, BlackPieces, WhitePieces) != NULL && FindPiece(x - n, y - n, BlackPieces, WhitePieces)->color == sf::Color::White) {
                        break;
                    }
                }
                l = n + 1;
                if (x - l > 0 || y - l > 0)
                {
                    if (!FindPiece(x - l, y - l, BlackPieces, WhitePieces) && FindPiece(x - n, y - n, BlackPieces, WhitePieces) != NULL && FindPiece(x - n, y - n, BlackPieces, WhitePieces)->color == sf::Color::White) {
                        e++;
                    }

                }

            }


        }
    }
    if (e == 0) {
        return -4;
    }
    else
    {


        return 0;
    }
}
//______________
int MovePiece(int x, int y, Piece* s_Piece, Piece* BlackPieces, Piece* WhitePieces, int* turn)
{
    if (s_Piece->color == sf::Color::Black && s_Piece->isKing == false) {//сначала шаги чёрных
        if (x == s_Piece->x - 1 && y == s_Piece->y - 1) {
            if (!FindPiece(x, y, BlackPieces, WhitePieces)) {//если поле пустое
                if (ObyazHod2(x, y, s_Piece, BlackPieces, WhitePieces, turn) == -2 && ObyazHodDama2(x, y, s_Piece, BlackPieces, WhitePieces, turn) == -4) {
                    *turn = ((*turn == 1) ? 2 : 1);//меняется очередь хода
                    s_Piece->x = x;
                    s_Piece->y = y;//обычный шаг чёрных в левы верхний на шаг угол
                    return 1;
                }
            }
        }
        if (x == s_Piece->x + 1 && y == s_Piece->y - 1) {
            if (!FindPiece(x, y, BlackPieces, WhitePieces)) {//если поле пустое
                if (ObyazHod2(x, y, s_Piece, BlackPieces, WhitePieces, turn) == -2 && ObyazHodDama2(x, y, s_Piece, BlackPieces, WhitePieces, turn) == -4) {
                    *turn = ((*turn == 1) ? 2 : 1);//меняется очередь хода
                    s_Piece->x = x;
                    s_Piece->y = y;//обычный шаг чёрных в левы верхний на шаг угол
                    return 1;
                }
            }
        }
        if (x == s_Piece->x - 2 && y == s_Piece->y - 2) {
            if (!FindPiece(x, y, BlackPieces, WhitePieces) && FindPiece(x + 1, y + 1, BlackPieces, WhitePieces) != NULL && FindPiece(x + 1, y + 1, BlackPieces, WhitePieces)->color != s_Piece->color) {
                *turn = ((*turn == 1) ? 2 : 1);
                KillPiece(x + 1, y + 1, BlackPieces, WhitePieces, turn);
                s_Piece->x = x;//собсна перемещение при поедании в левый верхний на два шага угол
                s_Piece->y = y;
                if (ObyazHod2Cep(x, y, s_Piece, BlackPieces, WhitePieces, turn) == 0) {
                    *turn = ((*turn == 1) ? 2 : 1);
                }
                Win(x, y, BlackPieces, WhitePieces, turn);
                return 1;
            }
        }
        if (x == s_Piece->x + 2 && y == s_Piece->y - 2) {
            if (!FindPiece(x, y, BlackPieces, WhitePieces) && FindPiece(x - 1, y + 1, BlackPieces, WhitePieces) != NULL && FindPiece(x - 1, y + 1, BlackPieces, WhitePieces)->color != s_Piece->color) {
                *turn = ((*turn == 1) ? 2 : 1);
                KillPiece(x - 1, y + 1, BlackPieces, WhitePieces, turn);
                s_Piece->x = x;
                s_Piece->y = y;//перемещение при поедании в правый верхний на два шага угол
                if (ObyazHod2Cep(x, y, s_Piece, BlackPieces, WhitePieces, turn) == 0) {
                    *turn = ((*turn == 1) ? 2 : 1);
                }
                Win(x, y, BlackPieces, WhitePieces, turn);
                return 1;
            }
        }
        //
        if (x == s_Piece->x - 2 && y == s_Piece->y + 2) {
            if (!FindPiece(x, y, BlackPieces, WhitePieces) && FindPiece(x + 1, y - 1, BlackPieces, WhitePieces) != NULL && FindPiece(x + 1, y - 1, BlackPieces, WhitePieces)->color != s_Piece->color) {
                *turn = ((*turn == 1) ? 2 : 1);
                KillPiece(x + 1, y - 1, BlackPieces, WhitePieces, turn);
                s_Piece->x = x;//собсна перемещение при поедании в левый верхний на два шага угол
                s_Piece->y = y;
                if (ObyazHod2Cep(x, y, s_Piece, BlackPieces, WhitePieces, turn) == 0) {
                    *turn = ((*turn == 1) ? 2 : 1);
                }
                Win(x, y, BlackPieces, WhitePieces, turn);
                return 1;
            }
        }
        if (x == s_Piece->x + 2 && y == s_Piece->y + 2) {
            if (!FindPiece(x, y, BlackPieces, WhitePieces) && FindPiece(x - 1, y - 1, BlackPieces, WhitePieces) != NULL && FindPiece(x - 1, y - 1, BlackPieces, WhitePieces)->color != s_Piece->color) {
                *turn = ((*turn == 1) ? 2 : 1);
                KillPiece(x - 1, y - 1, BlackPieces, WhitePieces, turn);
                s_Piece->x = x;
                s_Piece->y = y;//перемещение при поедании в правый верхний на два шага угол
                if (ObyazHod2Cep(x, y, s_Piece, BlackPieces, WhitePieces, turn) == 0) {
                    *turn = ((*turn == 1) ? 2 : 1);
                }
                Win(x, y, BlackPieces, WhitePieces, turn);
                return 1;
            }
        }
        //

    }
    if (s_Piece->color == sf::Color::White && s_Piece->isKing == false) {//шаги только белых

        if (x == s_Piece->x - 1 && y == s_Piece->y + 1) {
            if (!FindPiece(x, y, BlackPieces, WhitePieces)) {
                if (ObyazHod(x, y, s_Piece, BlackPieces, WhitePieces, turn) == -1 && ObyazHodDama(x, y, s_Piece, BlackPieces, WhitePieces, turn) == -3) {
                    *turn = ((*turn == 1) ? 2 : 1);//меняется очередь хода
                    s_Piece->x = x;
                    s_Piece->y = y;//обычный шаг чёрных в левы верхний на шаг угол
                    return 1;
                }
            }
        }
        if (x == s_Piece->x + 1 && y == s_Piece->y + 1) {
            if (!FindPiece(x, y, BlackPieces, WhitePieces)) {
                if (ObyazHod(x, y, s_Piece, BlackPieces, WhitePieces, turn) == -1 && ObyazHodDama(x, y, s_Piece, BlackPieces, WhitePieces, turn) == -3) {
                    *turn = ((*turn == 1) ? 2 : 1);//меняется очередь хода
                    s_Piece->x = x;
                    s_Piece->y = y;//обычный шаг чёрных в левы верхний на шаг угол
                    return 1;
                }
            }
        }
        if (x == s_Piece->x - 2 && y == s_Piece->y - 2) {
            if (!FindPiece(x, y, BlackPieces, WhitePieces) && FindPiece(x + 1, y + 1, BlackPieces, WhitePieces) != NULL && FindPiece(x + 1, y + 1, BlackPieces, WhitePieces)->color != s_Piece->color) {
                *turn = ((*turn == 1) ? 2 : 1);
                KillPiece(x + 1, y + 1, BlackPieces, WhitePieces, turn);
                s_Piece->x = x;//собсна перемещение при поедании в левый верхний на два шага угол
                s_Piece->y = y;
                if (ObyazHodCep(x, y, s_Piece, BlackPieces, WhitePieces, turn) == 0) {
                    *turn = ((*turn == 1) ? 2 : 1);
                }
                Win(x, y, BlackPieces, WhitePieces, turn);
                return 1;
            }
        }
        if (x == s_Piece->x + 2 && y == s_Piece->y - 2) {
            if (!FindPiece(x, y, BlackPieces, WhitePieces) && FindPiece(x - 1, y + 1, BlackPieces, WhitePieces) != NULL && FindPiece(x - 1, y + 1, BlackPieces, WhitePieces)->color != s_Piece->color) {
                *turn = ((*turn == 1) ? 2 : 1);
                KillPiece(x - 1, y + 1, BlackPieces, WhitePieces, turn);
                s_Piece->x = x;
                s_Piece->y = y;//перемещение при поедании в правый верхний на два шага угол
                if (ObyazHodCep(x, y, s_Piece, BlackPieces, WhitePieces, turn) == 0) {
                    *turn = ((*turn == 1) ? 2 : 1);
                }
                Win(x, y, BlackPieces, WhitePieces, turn);
                return 1;
            }
        }
        //
        if (x == s_Piece->x - 2 && y == s_Piece->y + 2) {
            if (!FindPiece(x, y, BlackPieces, WhitePieces) && FindPiece(x + 1, y - 1, BlackPieces, WhitePieces) != NULL && FindPiece(x + 1, y - 1, BlackPieces, WhitePieces)->color != s_Piece->color) {
                *turn = ((*turn == 1) ? 2 : 1);
                KillPiece(x + 1, y - 1, BlackPieces, WhitePieces, turn);
                s_Piece->x = x;//собсна перемещение при поедании в левый верхний на два шага угол
                s_Piece->y = y;
                if (ObyazHodCep(x, y, s_Piece, BlackPieces, WhitePieces, turn) == 0) {
                    *turn = ((*turn == 1) ? 2 : 1);
                }
                Win(x, y, BlackPieces, WhitePieces, turn);
                return 1;
            }
        }
        if (x == s_Piece->x + 2 && y == s_Piece->y + 2) {
            if (!FindPiece(x, y, BlackPieces, WhitePieces) && FindPiece(x - 1, y - 1, BlackPieces, WhitePieces) != NULL && FindPiece(x - 1, y - 1, BlackPieces, WhitePieces)->color != s_Piece->color) {
                *turn = ((*turn == 1) ? 2 : 1);
                KillPiece(x - 1, y - 1, BlackPieces, WhitePieces, turn);
                s_Piece->x = x;
                s_Piece->y = y;//перемещение при поедании в правый верхний на два шага угол
                if (ObyazHodCep(x, y, s_Piece, BlackPieces, WhitePieces, turn) == 0) {
                    *turn = ((*turn == 1) ? 2 : 1);
                }
                Win(x, y, BlackPieces, WhitePieces, turn);
                return 1;
            }
        }
        //
    }
    return 0;
}
//______________
int moveDama(int x, int y, Piece* s_Piece, Piece* BlackPieces, Piece* WhitePieces, int* turn)
{

    if (s_Piece->isKing == true && ((s_Piece->color == sf::Color::White && *turn == 2) || (s_Piece->color == sf::Color::Black && *turn == 1)))
    {

        int m = 0;//количество клеток сколько она должна пройти
        int k = 0;//количество клеток свободных на пути к кликнутой клетке
        int n = 1;//количество свободных клеток до преграды(какой-либо шашки)
        for (int i = 0; i < 8; i++)
        {
            if (!FindPiece(x + n, y + n, BlackPieces, WhitePieces))
            {
                n++;

            }
            else
            {
                break;
            }
        }


        //-----------------------------------------------------------------------------------------------------------------------------
        for (int i = 0; i < 8; i++) {
            m++;
            if (!FindPiece(s_Piece->x - m, s_Piece->y - m, WhitePieces, BlackPieces)) {
                k++;
            }
            if (x == s_Piece->x - m && y == s_Piece->y - m) {

                if (!FindPiece(x, y, BlackPieces, WhitePieces))
                {
                    if (k == m)
                    {
                        if ((s_Piece->color == sf::Color::White))
                        {
                            if (ObyazHod(x, y, s_Piece, BlackPieces, WhitePieces, turn) == -1 && ObyazHodDama(x, y, s_Piece, BlackPieces, WhitePieces, turn) == -3) {
                                *turn = ((*turn == 1) ? 2 : 1);
                                s_Piece->x = x;
                                s_Piece->y = y;
                                break;
                            }
                        }
                        if ((s_Piece->color == sf::Color::Black))
                        {
                            if (ObyazHod2(x, y, s_Piece, BlackPieces, WhitePieces, turn) == -2 && ObyazHodDama2(x, y, s_Piece, BlackPieces, WhitePieces, turn) == -4) {
                                *turn = ((*turn == 1) ? 2 : 1);
                                s_Piece->x = x;
                                s_Piece->y = y;
                                break;
                            }
                        }
                    }

                    if (k == (m - 1)) {
                        if (FindPiece(x + n, y + n, BlackPieces, WhitePieces) != NULL && FindPiece(x + n, y + n, BlackPieces, WhitePieces)->color == sf::Color::White)
                        {
                            *turn = ((*turn == 1) ? 2 : 1);
                            s_Piece->x = x;//перемещение при поедании в правый нижний на 2 шага угол
                            s_Piece->y = y;
                            KillPiece(x + n, y + n, BlackPieces, WhitePieces, turn);
                            if (ObyazHodDama2Cep(x, y, s_Piece, BlackPieces, WhitePieces, turn) == 0) {
                                *turn = ((*turn == 1) ? 2 : 1);
                            }
                            Win(x, y, BlackPieces, WhitePieces, turn);
                            return 1;
                            break;
                        }
                        else
                        {
                            break;
                        }
                        if (FindPiece(x + n, y + n, BlackPieces, WhitePieces) != NULL && FindPiece(x + n, y + n, BlackPieces, WhitePieces)->color == sf::Color::Black)
                        {
                            *turn = ((*turn == 1) ? 2 : 1);
                            s_Piece->x = x;//перемещение при поедании в правый нижний на 2 шага угол
                            s_Piece->y = y;
                            KillPiece(x + n, y + n, BlackPieces, WhitePieces, turn);
                            if (ObyazHodDamaCep(x, y, s_Piece, BlackPieces, WhitePieces, turn) == 0) {
                                *turn = ((*turn == 1) ? 2 : 1);
                            }
                            Win(x, y, BlackPieces, WhitePieces, turn);
                            return 1;
                            break;
                        }
                        else
                        {
                            break;
                        }
                    }

                }
            }

        }


        //__


        m = 0;
        k = 0;
        n = 1;

        for (int i = 0; i < 8; i++)
        {
            if (!FindPiece(x - n, y - n, BlackPieces, WhitePieces))
            {
                n++;

            }
            else
            {
                break;
            }
        }


        //-----------------------------------------------------------------------------------------------------------------------------
        m = 0;
        k = 0;
        for (int i = 0; i < 8; i++) {
            m++;
            if (!FindPiece(s_Piece->x + m, s_Piece->y + m, WhitePieces, BlackPieces)) {
                k++;
            }
            if (x == s_Piece->x + m && y == s_Piece->y + m) {
                if (!FindPiece(x, y, BlackPieces, WhitePieces))
                {
                    if (k == m)
                    {
                        if ((s_Piece->color == sf::Color::White))
                        {
                            if (ObyazHod(x, y, s_Piece, BlackPieces, WhitePieces, turn) == -1 && ObyazHodDama(x, y, s_Piece, BlackPieces, WhitePieces, turn) == -3) {
                                *turn = ((*turn == 1) ? 2 : 1);
                                s_Piece->x = x;
                                s_Piece->y = y;
                                break;
                            }
                        }
                        if ((s_Piece->color == sf::Color::Black))
                        {
                            if (ObyazHod2(x, y, s_Piece, BlackPieces, WhitePieces, turn) == -2 && ObyazHodDama2(x, y, s_Piece, BlackPieces, WhitePieces, turn) == -4) {
                                *turn = ((*turn == 1) ? 2 : 1);
                                s_Piece->x = x;
                                s_Piece->y = y;
                                break;
                            }
                        }
                    }

                    if (k == (m - 1)) {
                        if (FindPiece(x - n, y - n, BlackPieces, WhitePieces) != NULL && FindPiece(x - n, y - n, BlackPieces, WhitePieces)->color == sf::Color::White)
                        {
                            *turn = ((*turn == 1) ? 2 : 1);
                            s_Piece->x = x;//перемещение при поедании в правый нижний на 2 шага угол
                            s_Piece->y = y;
                            KillPiece(x - n, y - n, BlackPieces, WhitePieces, turn);
                            if (ObyazHodDama2Cep(x, y, s_Piece, BlackPieces, WhitePieces, turn) == 0) {
                                *turn = ((*turn == 1) ? 2 : 1);
                            }
                            Win(x, y, BlackPieces, WhitePieces, turn);
                            return 1;
                            break;
                        }
                        else
                        {
                            break;
                        }
                        if (FindPiece(x - n, y - n, BlackPieces, WhitePieces) != NULL && FindPiece(x - n, y - n, BlackPieces, WhitePieces)->color == sf::Color::Black)
                        {
                            *turn = ((*turn == 1) ? 2 : 1);
                            s_Piece->x = x;//перемещение при поедании в правый нижний на 2 шага угол
                            s_Piece->y = y;
                            KillPiece(x - n, y - n, BlackPieces, WhitePieces, turn);
                            if (ObyazHodDamaCep(x, y, s_Piece, BlackPieces, WhitePieces, turn) == 0) {
                                *turn = ((*turn == 1) ? 2 : 1);
                            }
                            Win(x, y, BlackPieces, WhitePieces, turn);
                            return 1;
                            break;
                        }
                        else
                        {
                            break;
                        }
                    }

                }
            }


        }
        //_______________

        m = 0;
        k = 0;
        n = 1;
        for (int i = 0; i < 8; i++)
        {
            if (!FindPiece(x + n, y - n, BlackPieces, WhitePieces))
            {
                n++;

            }
            else
            {
                break;
            }
        }



        //---------------------------------------------------------------------------------------------------------------------------------------

        m = 0;
        k = 0;
        for (int i = 0; i < 8; i++) {
            m++;
            if (!FindPiece(s_Piece->x - m, s_Piece->y + m, WhitePieces, BlackPieces)) {
                k++;
            }
            if (x == s_Piece->x - m && y == s_Piece->y + m) {
                if (!FindPiece(x, y, BlackPieces, WhitePieces))
                {


                    if (k == m) {
                        if ((s_Piece->color == sf::Color::White))
                        {
                            if (ObyazHod(x, y, s_Piece, BlackPieces, WhitePieces, turn) == -1 && ObyazHodDama(x, y, s_Piece, BlackPieces, WhitePieces, turn) == -3) {
                                *turn = ((*turn == 1) ? 2 : 1);
                                s_Piece->x = x;
                                s_Piece->y = y;
                                break;
                            }
                        }
                        if ((s_Piece->color == sf::Color::Black))
                        {
                            if (ObyazHod2(x, y, s_Piece, BlackPieces, WhitePieces, turn) == -2 && ObyazHodDama2(x, y, s_Piece, BlackPieces, WhitePieces, turn) == -4) {
                                *turn = ((*turn == 1) ? 2 : 1);
                                s_Piece->x = x;
                                s_Piece->y = y;
                                break;
                            }
                        }
                    }

                    if (k == (m - 1)) {
                        if (k == (m - 1)) {
                            if (FindPiece(x + n, y - n, BlackPieces, WhitePieces) != NULL && FindPiece(x + n, y - n, BlackPieces, WhitePieces)->color == sf::Color::White)
                            {
                                *turn = ((*turn == 1) ? 2 : 1);
                                s_Piece->x = x;//перемещение при поедании в правый нижний на 2 шага угол
                                s_Piece->y = y;
                                KillPiece(x + n, y - n, BlackPieces, WhitePieces, turn);
                                if (ObyazHodDama2Cep(x, y, s_Piece, BlackPieces, WhitePieces, turn) == 0) {
                                    *turn = ((*turn == 1) ? 2 : 1);
                                }
                                Win(x, y, BlackPieces, WhitePieces, turn);
                                return 1;
                                break;
                            }
                            else
                            {
                                break;
                            }
                            if (FindPiece(x + n, y - n, BlackPieces, WhitePieces) != NULL && FindPiece(x + n, y - n, BlackPieces, WhitePieces)->color == sf::Color::Black)
                            {
                                *turn = ((*turn == 1) ? 2 : 1);
                                s_Piece->x = x;//перемещение при поедании в правый нижний на 2 шага угол
                                s_Piece->y = y;
                                KillPiece(x + n, y - n, BlackPieces, WhitePieces, turn);
                                if (ObyazHodDamaCep(x, y, s_Piece, BlackPieces, WhitePieces, turn) == 0) {
                                    *turn = ((*turn == 1) ? 2 : 1);
                                }
                                Win(x, y, BlackPieces, WhitePieces, turn);
                                return 1;
                                break;
                            }
                            else
                            {
                                break;
                            }
                        }
                    }

                }

            }

        }
        m = 0;
        k = 0;
        n = 1;

        //__
        for (int i = 0; i < 8; i++)
        {
            if (!FindPiece(x - n, y + n, WhitePieces, BlackPieces))
            {
                n++;

            }
            else
            {
                break;
            }
        }

        //-----------------------------------------------------------------------------------------------------------------------------
        m = 0;
        k = 0;
        for (int i = 0; i < 8; i++) {
            m++;

            if (!FindPiece(s_Piece->x + m, s_Piece->y - m, WhitePieces, BlackPieces))
            {
                k++;
            }

            if (x == s_Piece->x + m && y == s_Piece->y - m)
            {
                if (!FindPiece(x, y, BlackPieces, WhitePieces))
                {
                    if (k == m)
                    {
                        if ((s_Piece->color == sf::Color::White))
                        {
                            if (ObyazHod(x, y, s_Piece, BlackPieces, WhitePieces, turn) == -1 && ObyazHodDama(x, y, s_Piece, BlackPieces, WhitePieces, turn) == -3) {
                                *turn = ((*turn == 1) ? 2 : 1);
                                s_Piece->x = x;
                                s_Piece->y = y;
                                break;
                            }
                        }
                        if ((s_Piece->color == sf::Color::Black))
                        {
                            if (ObyazHod2(x, y, s_Piece, BlackPieces, WhitePieces, turn) == -2 && ObyazHodDama2(x, y, s_Piece, BlackPieces, WhitePieces, turn) == -4) {
                                *turn = ((*turn == 1) ? 2 : 1);
                                s_Piece->x = x;
                                s_Piece->y = y;
                                break;
                            }
                        }
                    }

                    if (k == (m - 1)) {
                        if (k == (m - 1)) {
                            if (FindPiece(x - n, y + n, BlackPieces, WhitePieces) != NULL && FindPiece(x - n, y + n, BlackPieces, WhitePieces)->color == sf::Color::White)
                            {
                                *turn = ((*turn == 1) ? 2 : 1);
                                s_Piece->x = x;//перемещение при поедании в правый нижний на 2 шага угол
                                s_Piece->y = y;
                                KillPiece(x - n, y + n, BlackPieces, WhitePieces, turn);
                                if (ObyazHodDama2Cep(x, y, s_Piece, BlackPieces, WhitePieces, turn) == 0) {
                                    *turn = ((*turn == 1) ? 2 : 1);
                                }
                                Win(x, y, BlackPieces, WhitePieces, turn);
                                return 1;
                                break;
                            }
                            else
                            {
                                break;
                            }
                            if (FindPiece(x - n, y + n, BlackPieces, WhitePieces) != NULL && FindPiece(x - n, y + n, BlackPieces, WhitePieces)->color == sf::Color::Black)
                            {
                                *turn = ((*turn == 1) ? 2 : 1);
                                s_Piece->x = x;//перемещение при поедании в правый нижний на 2 шага угол
                                s_Piece->y = y;
                                KillPiece(x - n, y + n, BlackPieces, WhitePieces, turn);
                                if (ObyazHodDamaCep(x, y, s_Piece, BlackPieces, WhitePieces, turn) == 0) {
                                    *turn = ((*turn == 1) ? 2 : 1);
                                }
                                Win(x, y, BlackPieces, WhitePieces, turn);
                                return 1;
                                break;
                            }
                            else
                            {
                                break;
                            }
                        }
                    }
                }

            }
        }
    }
    return 0;

}
//______________
int main()
{
    setlocale(LC_ALL, "ru");
    sf::ContextSettings settings;
    settings.antialiasingLevel = 16;
sf::RenderWindow window(sf::VideoMode(600, 600), "Checkers", sf::Style::Default, settings);//создание главного окна
    sf::Event event;
    Board board;
    Piece BlackPieces[12];
    Piece WhitePieces[12];
    bool selected = false;
    Piece* SelectedPiece = NULL;
    int turn = 2;

    for (int i = 0; i < 12; i++) {
        WhitePieces[i].color = sf::Color::White;//набираем сет из  шашек
        BlackPieces[i].color = sf::Color::Black;
    }


    Setup(window, BlackPieces, WhitePieces);

    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonReleased) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    selected = true;
                }
            }
        }

        window.clear();

        board.Draw(window);

        if (SelectedPiece != NULL) {
            board.Highlight(window, SelectedPiece->x, SelectedPiece->y);
        }

        for (int i = 0; i < 12; i++) {
            WhitePieces[i].Draw(window);
            BlackPieces[i].Draw(window);
        }

        if (selected) {
            int x = sf::Mouse::getPosition(window).x / 75;
            int y = sf::Mouse::getPosition(window).y / 75;
            if (FindPiece(x, y, BlackPieces, WhitePieces) && (FindPiece(x, y, BlackPieces, WhitePieces)->color == sf::Color::Black && turn == 1 || FindPiece(x, y, BlackPieces, WhitePieces)->color == sf::Color::White && turn == 2)) {
                if (FindPiece(x, y, BlackPieces, WhitePieces) == NULL) {
                    SelectedPiece = NULL;
                }
                else {
                    SelectedPiece = FindPiece(x, y, BlackPieces, WhitePieces);
                }

                selected = false;
            }
            else if (SelectedPiece != NULL && MovePiece(x, y, SelectedPiece, BlackPieces, WhitePieces, &turn)) {
                selected = false;
                SelectedPiece = NULL;
            }
            else if ((SelectedPiece != NULL && SelectedPiece->isKing == true) && moveDama(x, y, SelectedPiece, BlackPieces, WhitePieces, &turn)) {
                selected = false;
                SelectedPiece = NULL;
            }
            selected = false;
        }
        for (int i = 0; i < 12; i++) {
            if (BlackPieces[i].y == 0) {
                BlackPieces[i].isKing = true;
            }
            if (WhitePieces[i].y == 7) {
                WhitePieces[i].isKing = true;
            }
        }


        window.display();
    }
    main();
    return 0;
}