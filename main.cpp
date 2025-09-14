#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <filesystem>
#include <locale>
#include <codecvt>

using namespace std;
using namespace sf;

struct TransportInfo {
    wstring route;
    wstring type;
    int capacity;
    wstring driver;
    wstring schedule;
};

const int MAX_ROWS = 6;
const int MAX_COLS = 5;
TransportInfo transportData[MAX_ROWS];
int rowCount = 0;

std::filesystem::path resourceDir;

wstring input(RenderWindow& window, String hint) {
    Font font;
    if (!font.loadFromFile((resourceDir / "Arial.ttf").string())) {
        wcerr << L"Ошибка загрузки шрифта." << endl;
        return L"";
    }

    Text text(hint, font, 40);
    text.setFillColor(Color::White);
    text.setPosition(10, 10);

    String input;
    bool enterPressed = false;

    while (window.isOpen() && !enterPressed) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
                return L"";
            }

            if (event.type == Event::TextEntered) {
                if (event.text.unicode == '\b' && !input.isEmpty()) {
                    input.erase(input.getSize() - 1);
                } else if (event.text.unicode == '\r' || event.text.unicode == '\n') {
                    enterPressed = true;
                } else if (event.text.unicode < 128 || (event.text.unicode >= 1040 && event.text.unicode <= 1103)) {
                    input += event.text.unicode;
                }
            }
        }

        window.clear();
        window.draw(text);

        Text inputText(input, font, 40);
        inputText.setFillColor(Color::White);
        inputText.setPosition(10, 60);
        window.draw(inputText);

        window.display();
    }

    return input.toWideString();
}

void showTable(RenderWindow& window, String buttonLabel = L"Назад") {
    Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile((resourceDir / "Avtiki.jpg").string())) {
        wcerr << L"Ошибка загрузки текстуры изображения." << endl;
        return;
    }

    Sprite backgroundSprite(backgroundTexture);
    backgroundSprite.setPosition(0, 0);

    window.clear(Color::Black);
    window.draw(backgroundSprite);

    Font font;
    if (!font.loadFromFile((resourceDir / "Arial.ttf").string())) {
        wcerr << L"Ошибка загрузки шрифта." << endl;
        return;
    }

    Text title(L"Таблица", font, 50);
    title.setPosition(400, 40);
    window.draw(title);

    Text header(L"Маршрут         Тип       Вместимость   Водитель Расписание", font, 36);
    header.setPosition(25, 110);
    window.draw(header);

    Text data[MAX_ROWS][MAX_COLS];
    for (int i = 0; i < rowCount; i++) {
        data[i][0].setString(transportData[i].route);
        data[i][1].setString(transportData[i].type);
        data[i][2].setString(to_wstring(transportData[i].capacity));
        data[i][3].setString(transportData[i].driver);
        data[i][4].setString(transportData[i].schedule);
        for (int j = 0; j < MAX_COLS; j++) {
            data[i][j].setFont(font);
            data[i][j].setCharacterSize(30);
            if (j == 0) {
                data[i][j].setPosition(70, 165 + i * 60);
            } else if (j == 1) {
                data[i][j].setPosition(220, 165 + i * 60);
            } else {
                data[i][j].setPosition(290 + (j - 1) * 190, 165 + i * 60);
            }
            window.draw(data[i][j]);
        }
    }

    Text backButton(buttonLabel, font, 50);
    backButton.setPosition(430, 670);
    window.draw(backButton);

    window.display();

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
                return;
            }
            if (event.type == Event::MouseButtonPressed) {
                Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
                if (backButton.getGlobalBounds().contains(mousePos)) return;
            }
        }
    }
}

void addData(RenderWindow& window) {
    if (rowCount < MAX_ROWS) {
        wcout << endl << L"Добавление записи:" << endl;

        transportData[rowCount].route = input(window, L"Введите номер маршрута:");
        transportData[rowCount].type = input(window, L"Введите тип транспорта:");

        wstring capacityInput = input(window, L"Введите вместимость:");
        if (capacityInput.empty()) {
            wcout << L"Вместимость не указана." << endl;
            return;
        }
        try {
            transportData[rowCount].capacity = stoi(capacityInput);
        } catch(...) {
            wcout << L"Вместимость указана не корректно. Введите вместимость цифрами." << endl;
            return;
        }

        transportData[rowCount].driver = input(window, L"Введите фамилию водителя:");
        transportData[rowCount].schedule = input(window, L"Введите расписание работы транспорта:");

        rowCount++;
    } else {
        wcout << L"Достигнуто максимальное количество записей." << endl;
    }
}

void removeData(RenderWindow& window) {
    if (rowCount > 0) {
        showTable(window, L"Далее");

        wstring rowToDeleteStr = input(window, L"Введите номер строки для удаления:");
        if (rowToDeleteStr.empty()) return;

        int rowToDelete;
        try {
            rowToDelete = stoi(rowToDeleteStr);
        } catch (...) {
            wcout << L"Неверный формат номера строки." << endl;
            return;
        }

        if (rowToDelete > 0 && rowToDelete <= rowCount) {
            for (int i = rowToDelete - 1; i < rowCount - 1; i++) {
                transportData[i] = transportData[i + 1];
            }
            rowCount--;
            wcout << L"Запись удалена успешно." << endl;
        } else {
            wcout << L"Неверный номер строки." << endl;
        }
    } else {
        wcout << L"Таблица пуста." << endl;
    }
}

void editData(RenderWindow& window) {
    if (rowCount > 0) {
        showTable(window, L"Далее");

        wstring rowToEditStr = input(window, L"Введите номер строки для редактирования:");
        if (rowToEditStr.empty()) return;

        int rowToEdit;
        try {
            rowToEdit = stoi(rowToEditStr);
        } catch (const invalid_argument& e) {
            wcout << L"Неверный формат номера строки." << endl;
            return;
        }

        if (rowToEdit > 0 && rowToEdit <= rowCount) {
            transportData[rowToEdit - 1].route = input(window, L"Маршрут:");
            transportData[rowToEdit - 1].type = input(window, L"Тип:");

            wstring capacityInput = input(window, L"Введите вместимость:");
            if (capacityInput.empty()) {
                wcout << L"Вместимость не указана." << endl;
                return;
            }
            try {
                transportData[rowCount].capacity = stoi(capacityInput);
            } catch(...) {
                wcout << L"Вместимость не корректно. Введите вместимость числом." << endl;
                return;
            }

            transportData[rowToEdit - 1].driver = input(window, L"Водитель:");
            transportData[rowToEdit - 1].schedule = input(window, L"Расписание:");

            wcout << L"Запись отредактирована успешно." << endl;
        } else {
            wcout << L"Неверный номер строки." << endl;
        }
    } else {
        wcout << L"Таблица пуста." << endl;
    }
}

char selectColumn(RenderWindow& window) {
    Font font;
    if (!font.loadFromFile((resourceDir / "Arial.ttf").string())) {
        wcerr << L"Ошибка загрузки шрифта." << endl;
        return '\0';
    }

    Text options[5];
    wstring optionStrings[] = { L"По маршруту", L"По типу", L"По вместимости", L"По водителю", L"По расписанию" };
    for (int i = 0; i < 5; i++) {
        options[i].setString(optionStrings[i]);
        options[i].setFont(font);
        options[i].setCharacterSize(50);
        options[i].setPosition(310, 200 + i * 60);
    }

    window.clear(Color::Black);
    for (int i = 0; i < 5; i++) {
        window.draw(options[i]);
    }
    window.display();

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
                return '\0';
            }
            if (event.type == Event::MouseButtonPressed) {
                Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
                for (int i = 0; i < 5; i++) {
                    if (options[i].getGlobalBounds().contains(mousePos)) {
                        switch (i) {
                            case 0: return 'M';
                            case 1: return 'T';
                            case 2: return 'B';
                            case 3: return 'D';
                            case 4: return 'P';
                        }
                    }
                }
            }
        }
    }
    return '\0';
}

void searching(RenderWindow& window) {
    char choice = selectColumn(window);
    if (choice == '\0') return;

    wstring searchTerm;
    bool found = false;

    Font font;
    if (!font.loadFromFile((resourceDir / "Arial.ttf").string())) {
        wcerr << L"Ошибка загрузки шрифта." << endl;
        return;
    }

    searchTerm = input(window, L"Введите значение для поиска:");

    Text resultText("", font, 30);
    resultText.setFillColor(Color::White);
    resultText.setPosition(50, 300);

    window.clear();
    int displayLine = 0;
    for (int i = 0; i < rowCount; i++) {
        bool match = false;
        switch (choice) {
            case 'M': match = (transportData[i].route == searchTerm); break;
            case 'T': match = (transportData[i].type == searchTerm); break;
            case 'B': match = (to_wstring(transportData[i].capacity) == searchTerm); break;
            case 'D': match = (transportData[i].driver == searchTerm); break;
            case 'P': match = (transportData[i].schedule == searchTerm); break;
        }
        if (match) {
            found = true;
            resultText.setString(L"Найдено в строке " + to_wstring(i + 1) + L": " +
                transportData[i].route + L" " +
                transportData[i].type + L" " +
                to_wstring(transportData[i].capacity) + L" " +
                transportData[i].driver + L" " +
                transportData[i].schedule);
            resultText.setPosition(50, 50 + displayLine * 50);
            window.draw(resultText);
            displayLine++;
        }
    }

    if (!found) {
        resultText.setString(L"Запись не найдена.");
        resultText.setPosition(50, 300);
        window.draw(resultText);
    }

    window.display();

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
                return;
            }
            if (event.type == Event::KeyPressed) {
                return;
            }
        }
    }
}

void sorting(RenderWindow& window) {
    char choice = selectColumn(window);
    if (choice == '\0') return;

    bool swapped;
    do {
        swapped = false;
        for (int i = 0; i < rowCount - 1; i++) {
            switch (choice) {
                case 'M':
                    if (transportData[i].route > transportData[i + 1].route) {
                        swap(transportData[i], transportData[i + 1]);
                        swapped = true;
                    }
                    break;
                case 'T':
                    if (transportData[i].type > transportData[i + 1].type) {
                        swap(transportData[i], transportData[i + 1]);
                        swapped = true;
                    }
                    break;
                case 'B':
                    if (transportData[i].capacity > transportData[i + 1].capacity) {
                        swap(transportData[i], transportData[i + 1]);
                        swapped = true;
                    }
                    break;
                case 'D':
                    if (transportData[i].driver > transportData[i + 1].driver) {
                        swap(transportData[i], transportData[i + 1]);
                        swapped = true;
                    }
                    break;

                case 'P':
                    if (transportData[i].schedule < transportData[i + 1].schedule) {
                        swap(transportData[i], transportData[i + 1]);
                        swapped = true;
                    }
                    break;
                default:
                    return;
            }
        }
    } while (swapped);

    wcout << L"Таблица отсортирована." << endl;
}

void fileReading() {
    wifstream file(resourceDir / "transport.txt");
    file.imbue(locale(locale(), new codecvt_utf8<wchar_t>));
    if (file.is_open()) {
        while (rowCount < MAX_ROWS && !file.eof()) {
            getline(file, transportData[rowCount].route, L' ');
            getline(file, transportData[rowCount].type, L' ');
            file >> transportData[rowCount].capacity;
            file.ignore();
            getline(file, transportData[rowCount].driver, L' ');
            getline(file, transportData[rowCount].schedule);
            if (transportData[rowCount].capacity > 0) rowCount++;
        }
        file.close();
        wcout << L"Данные загрузились из файла." << endl;
    } else {
        wcout << L"Ошибка открытия файла." << endl;
    }
}

void fileWriting() {
    wofstream file(resourceDir / "transport.txt");
    file.imbue(locale(locale(), new codecvt_utf8<wchar_t>));
    if (file.is_open()) {
        for (int i = 0; i < rowCount; i++) {
            file << transportData[i].route << L' '
                 << transportData[i].type << L' '
                 << transportData[i].capacity << L' '
                 << transportData[i].driver << L' '
                 << transportData[i].schedule << endl;
        }
        file.close();
        wcout << L"Данные сохранены в файл." << endl;
    } else {
        wcout << L"Ошибка открытия файла." << endl;
    }
}

void mainMenu(RenderWindow& window) {
    Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile((resourceDir / "Avtiki.jpg").string())) {
        wcerr << L"Ошибка загрузки текстуры изображения." << endl;
        return;
    }

    Sprite backgroundSprite(backgroundTexture);
    backgroundSprite.setPosition(0, 0);

    window.clear(Color::Black);
    window.draw(backgroundSprite);

    Font font;
    if (!font.loadFromFile((resourceDir / "Arial.ttf").string())) {
        wcerr << L"Ошибка загрузки шрифта." << endl;
        return;
    }

    Text title(L"Общественный транспорт", font, 60);
    title.setPosition(145, 50);
    window.draw(title);

    Text options[8];
    wstring optionStrings[] = { L"Просмотр таблицы", L"Добавить запись", L"Удалить запись", L"Редактировать запись", L"Сортировать записи", L"Поиск записей", L"Сохранить в файл", L"Выход" };
    for (int i = 0; i < 8; i++) {
        options[i].setString(optionStrings[i]);
        options[i].setFont(font);
        options[i].setCharacterSize(50);
        options[i].setPosition(270, 150 + i * 60);
        window.draw(options[i]);
    }

    window.display();

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
                return;
            }
            if (event.type == Event::MouseButtonPressed) {
                Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
                for (int i = 0; i < 8; i++) {
                    if (options[i].getGlobalBounds().contains(mousePos)) {
                        switch (i) {
                            case 0:
                                showTable(window);
                                break;
                            case 1:
                                addData(window);
                                break;
                            case 2:
                                removeData(window);
                                break;
                            case 3:
                                editData(window);
                                break;
                            case 4:
                                sorting(window);
                                break;
                            case 5:
                                searching(window);
                                break;
                            case 6:
                                fileWriting();
                                break;
                            case 7:
                                window.close();
                                return;
                        }
                        mainMenu(window);
                    }
                }
            }
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc > 0) {
        resourceDir = std::filesystem::path(argv[0]).parent_path();
    }

    fileReading();

    RenderWindow window(VideoMode(1024, 768), L"Информационно-поисковая система Общественный Транспорт");
    window.setFramerateLimit(60);

    mainMenu(window);

    return 0;
}
