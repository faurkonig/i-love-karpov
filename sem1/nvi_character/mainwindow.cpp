#include "mainwindow.h"
#include "ui_mainwindow.h"

// Переменные для хранения палитр
static QPalette redPalette; // Палитра для красного текста
static QPalette normPalette; // Палитра для обычного текста

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Создание палитры с красный цветом текста
    redPalette = ui->pointsValue->palette();
    redPalette.setColor(QPalette::Text, Qt::red);

    // Создание палитры со стандартным цветом текста
    // Я использую не обычный чёрный цвет, так как в тёмной теме системы (и следовательно в тёмной теме Qt)
    // цвет уже другого цвета. У меня на компьютере, например, он, наоборот, белый,
    // однако может быть даже каким-нибудь голубоватым
    normPalette = ui->pointsValue->palette();
    QColor defaultTextColor = ui->nameLabel->palette().color(QPalette::Text);
    normPalette.setColor(QPalette::Text, defaultTextColor);

    ui->clearButton->click();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Константный массив с классами (сделан просто для упрощения)
const QString classes [14] = {
    "Берсерк", "Амазонка",
    "Маг", "Ведьма",
    "Ниндзя", "Охотница",
    "Некромант", "Призывательница",
    "Клирик", "Зачаровательница",
    "Ассасин", "Воровка",
    "Шулер", "Иллюзионистка"
};

void MainWindow::on_clearButton_clicked()
{
    // Очищение полей результата
    ui->hpValue->clear();
    ui->mpValue->clear();
    ui->attackValue->clear();
    ui->defenseValue->clear();
    ui->classValue->clear();

    // Очищение полей входных значений
    ui->streightValue->clear();
    ui->dexterityValue->clear();
    ui->intelligenceValue->clear();
    ui->luckValue->clear();

    // Очищение поля с именем
    ui->nameValue->clear();

    // Очищение выбранного пола
    // "setAutoExclusive" нужен для того, чтобы они не переключались между собой, а просто "стирались"
    ui->genderRadioButtonM->setAutoExclusive(false);
    ui->genderRadioButtonW->setAutoExclusive(false);
    ui->genderRadioButtonM->setChecked(false);
    ui->genderRadioButtonW->setChecked(false);
    ui->genderRadioButtonM->setAutoExclusive(true);
    ui->genderRadioButtonW->setAutoExclusive(true);

    // Установка цвета всех полей входных значений на стандартный (чтобы ошибки
    ui->nameValue->setPalette(normPalette);
    ui->streightValue->setPalette(normPalette);
    ui->dexterityValue->setPalette(normPalette);
    ui->intelligenceValue->setPalette(normPalette);
    ui->luckValue->setPalette(normPalette);
    ui->pointsValue->setPalette(normPalette);

    // Сброс очков
    points = 20;
    ui->pointsValue->setText(QString::number(points));

    // Сброс цвета очков на экране (в случае, если перед сбросом были неверные значения)
    QPalette pal;
    pal = ui->pointsValue->palette();
    QColor defaultTextColor = ui->nameLabel->palette().color(QPalette::Text);
    pal.setColor(QPalette::Text, defaultTextColor);
    ui->pointsValue->setPalette(pal);
}

// Константный массив с ошибками для полей ввода параметров
const QString mainInputErrors [8] = {
    "Неверное значение параметра \"Сила\"",
    "\"Сила\" должна быть от 1 до 10",
    "Неверное значение параметра \"Ловкость\"",
    "\"Ловкость\" должна быть от 1 до 10",
    "Неверное значение параметра \"Интеллект\"",
    "\"Интеллект\" должна быть от 1 до 10",
    "Неверное значение параметра \"Удача\"",
    "\"Удача\" должна быть от 1 до 10"
};

void MainWindow::on_createButton_clicked()
{


    // Переменные для хранения входных значений
    QString name;
    int lenght;
    bool flag;
    int str, dex, intel, luck;
    bool isMale;

    // Ввод имени и обработка его значения
    name = ui->nameValue->text();
    lenght = name.length();
    if (lenght < 3 || lenght > 20)
    {
        flag = false;
        ui->nameValue->setPalette(redPalette);
        QMessageBox::warning(this, "Ошибка", "Длина имени должна быть от 3 до 20");
    }
    else {
        flag = true;
        ui->nameValue->setPalette(normPalette);
    }

    if (flag)
    {
        // Ввод пола и обработка его значения
        if (!ui->genderRadioButtonM->isChecked() && !ui->genderRadioButtonW->isChecked())
        {
            flag = false;
            QMessageBox::warning(this, "Ошибка", "Пол не выбран.");
        }
        else
        {
             flag = true;
             isMale = ui->genderRadioButtonM->isChecked();
        }
    }

    /* Обработка характеристик
     * Так как я сделал "подсветку" красным цветом для всех входных параметров, то
     * возникает проблема "наслаивания" ошибок, то есть продолжают "подсвечиваться" красным поля,
     * которые идут ошибочного, как на данной записе: https://drive.google.com/file/d/14AHxABOvMVjzGctsQ7EYoDElyCWQS7cn/view?usp=sharing
     * Для исправления этой ошибки я создаю несколько дополнительных полей в "середине" ввода
     * Конечное значение у меня всё равно будет красный (и с надписью "error"), если ввод по ходу проверки неправильный,
     * так что для него не надо делать данную проверку и данные флаги
     * Переменная "errCode" нужна для того, чтобы выводить ошибку после проверок и "перекрашивания" полей,
     * иначе цвета меняются только после ошибки, что не очень логично в данном случае.
     */
    bool ok1, ok2, ok3, ok4;
    int errCode = -1;

    // Ввод силы и обработка её значения
    str = ui->streightValue->text().toInt(&ok1);
    if (!ok1 && flag)
    {
        // Если изначальный ввод некорректный
        errCode = 0;
    }
    else if (str < 1 || str > 10)
    {
        // Если изначальный ввод всё-таки корректный, но число очков не находится в области [1; 10]
        ok1 = false;
        ui->streightValue->setPalette(redPalette);
        if (flag)
            errCode = 1;
    }
    // Если поле правильное, для него устанавливается стандартный цвет
    if (ok1)
        ui->streightValue->setPalette(normPalette);
    else
    {
        // Если же поле неправильное, то для него устанавливается красный цвет и переключается изначальный флаг
        ui->streightValue->setPalette(redPalette);
        flag = false;
    }

    // Ввод ловкости и обработка её значения
    dex = ui->dexterityValue->text().toInt(&ok2);
    if (!ok2 && flag)
    {
        // Если изначальный ввод некорректный
        errCode = 2;
    }
    else if (dex < 1 || dex > 10)
    {
        // Если изначальный ввод всё-таки корректный, но число очков не находится в области [1; 10]
        ok2 = false;
        ui->dexterityValue->setPalette(redPalette);
        if (flag)
            errCode = 3;
    }
    // Если поле правильное, для него устанавливается стандартный цвет
    if (ok2)
        ui->dexterityValue->setPalette(normPalette);
    else
    {
        // Если же поле неправильное, то для него устанавливается красный цвет и переключается изначальный флаг
        ui->dexterityValue->setPalette(redPalette);
        flag = false;
    }

    // Ввод интеллекта и обработка его значения
    intel = ui->intelligenceValue->text().toInt(&ok3);
    if (!ok3 && flag)
    {
        // Если изначальный ввод некорректный
        errCode = 4;
    }
    else if (intel < 1 || intel > 10)
    {
        // Если изначальный ввод всё-таки корректный, но число очков не находится в области [1; 10]
        ok3 = false;
        ui->intelligenceValue->setPalette(redPalette);
        if (flag)
            errCode = 5;
    }
    // Если поле правильное, для него устанавливается стандартный цвет
    if (ok3)
        ui->intelligenceValue->setPalette(normPalette);
    else
    {
        // Если же поле неправильное, то для него устанавливается красный цвет и переключается изначальный флаг
        ui->intelligenceValue->setPalette(redPalette);
        flag = false;
    }

    // Ввод удачи и обработка её значения
    luck = ui->luckValue->text().toInt(&ok4);
    if (!ok4 && flag)
    {
        // Если изначальный ввод некорректный
        errCode = 6;
    }
    else if (luck < 1 || luck > 10)
    {
        // Если изначальный ввод всё-таки корректный, но число очков не находится в области [1; 10]
        ok4 = false;
        ui->luckValue->setPalette(redPalette);
        if (flag)
            errCode = 7;
    }
    // Если поле правильное, для него устанавливается стандартный цвет
    if (ok4)
        ui->luckValue->setPalette(normPalette);
    else
    {
        // Если же поле неправильное, то для него устанавливается красный цвет и переключается изначальный флаг
        ui->luckValue->setPalette(redPalette);
        flag = false;
    }

    // Если все предыдущие поля имеют нормальные значения
    if (flag)
    {
        // Вычисление оставшихся очков и вывод их на экран
        int sum = points - (str + dex + intel + luck);
        ui->pointsValue->setText(QString::number(sum));

        // Переменная для хранения палитры (нужна для того, чтобы открашивать оставшиеся очки в красный цвет, если

        if (sum != 0)
        {
            // Если не все очки были потрачены или наоборот их потрачено больше доступного
            flag = false;
            ui->pointsValue->setPalette(redPalette);
            if (sum > 0)
            {
                QMessageBox::warning(this, "Ошибка", "Нужно потратить все очки");
            }
            else
            {
                QMessageBox::warning(this, "Ошибка", "Нельзя тратить больше, чем " + QString::number(points) + " очков.");
            }
        }
        else
            ui->pointsValue->setPalette(normPalette);
    }
    // Если не все предыдущие поля имею нормальные значения
    else
    {
        // Устанавливается цвет поля на красный и текст на "error"
        ui->pointsValue->setPalette(redPalette);
        ui->pointsValue->setText("error");
    }

    if (errCode >= 0)
        QMessageBox::warning(this, "Ошибка", mainInputErrors[errCode]);

    if (flag)
    {
        // Если все входные значения правильные и все очки правильно потрачены,
        // то вычисляются характеристик персонажа и его класс

        // Вычисление характеристик
        int health = 8 * str + 2 * dex + (isMale? 9 : 1);
        int mana = 8 * intel + dex + luck + (isMale? 3 : 7);
        int attk = 6 * str + 2 * dex + 2 * luck + 5;
        int def = 7 * dex + 3 * luck + (isMale? 7 : 3);

        QString className; // Переменная для хранения названия класса персонажа

        /* Схема выбора класса такова:
         * Есть 4 класса с "лидирующим" параметром, 3 класса с 2 параметрами чуть выше среднего,
         * класс-универсал, параметры которого либо расспределены примерно равномерно, либо есть более одного "лидирующего" параметра.
         * Также у кажного класса есть женский аналог. Теоретически он должен иметь другие способности, но должен быть схож с "мужским классом"
         * "лидирующий" параметр (далее "лид параметр") - параметр с очками от 8 и больше других
         * Eсли лид параметр 1 и это сила, то выбирается класс, основанный на силе: "Берсерк" или "Амазонка"
         * Если лид параметр 1 и это интеллект, то выбирается класс, основанный на интеллекте: "Маг" или "Ведьма"
         * Если лид параметр 1 и это ловкость, то выбирается класс, основанный на ловкости: "Ниндзя" или "Охотница"
         * Другие 3 класса имеют 2 параметра очки от 6 и при этом 3ий основной параметр (основные это сила, ловкость, интеллект)
         * меньше этих двух. Это более универсальные классы, однако всё же имеющие предпочтения к каким-либо конкретным параметрам
         * Если сила и ловкость >= 6, а интеллект меньше, то выбирается "Ассасин" или "Воровка"
         * Если сила и интеллект >= 6, а ловкость меньше, то выбирается "Некромант" или "Призывательница"
         * Если ловкость и интеллект >= 6, а сила меньше, то выбирается "Клирик" или "Зачаровательница"
         * А также есть 1 особенный класс, основанный на удаче: "Шулер" или "Иллюзионистка"
         *
         * Если же нет какого конкретного предпочтительного параметра у персонажа, то выбирается класс "Универсал"
         * Универсал подразумевает слабое использование навыков других классов, распределённые характеристики,
         * в общем возможность использования особенностей других классов но в ослабленном и распределённом виде
         */
        if (str >= 8 && str > dex && str > intel)
            className = classes[isMale? 0 : 1];
        else if (intel >= 8 && intel > str && intel > dex)
            className = classes[isMale? 2 : 3];
        else if (dex >= 8 && dex > str && dex > intel)
            className = classes[isMale? 4 : 5];
        else if (str >= 6 && intel >= 6 && dex < str && dex < intel)
            className = classes[isMale? 6 : 7];
        else if (dex >= 6 && intel >= 6 && str < dex && str < intel)
            className = classes[isMale? 8 : 9];
        else if (str >= 6 && dex >= 6 && intel < str && intel < dex)
            className = classes[isMale? 10 : 11];
        else if (luck >= 8 && luck > str && luck > dex && luck > intel)
            className = classes[isMale? 12 : 13];
        else className = isMale? "Универсал" : "Универсалка";

        // Вывод значений на экран
        ui->hpValue->setNum(health);
        ui->mpValue->setNum(mana);
        ui->attackValue->setNum(attk);
        ui->defenseValue->setNum(def);
        ui->classValue->setText(className);
    }
}


void MainWindow::on_nameValue_textChanged(const QString &arg1)
{
    if (arg1.length() < 3 || arg1.length() > 20)
        ui->nameValue->setPalette(redPalette);
    else
        ui->nameValue->setPalette(normPalette);
}

void MainWindow::on_streightValue_textChanged(const QString &arg1)
{
    bool ok;
    int number = arg1.toInt(&ok);

    if (!ok || number > 10 || number < 1)
        ui->streightValue->setPalette(redPalette);
    else
        ui->streightValue->setPalette(normPalette);
}

void MainWindow::on_dexterityValue_textChanged(const QString &arg1)
{
    bool ok;
    int number = arg1.toInt(&ok);

    if (!ok || number > 10 || number < 1)
        ui->dexterityValue->setPalette(redPalette);
    else
        ui->dexterityValue->setPalette(normPalette);
}

void MainWindow::on_intelligenceValue_textChanged(const QString &arg1)
{
    bool ok;
    int number = arg1.toInt(&ok);

    if (!ok || number > 10 || number < 1)
        ui->intelligenceValue->setPalette(redPalette);
    else
        ui->intelligenceValue->setPalette(normPalette);
}

void MainWindow::on_luckValue_textChanged(const QString &arg1)
{
    bool ok;
    int number = arg1.toInt(&ok);

    if (!ok || number > 10 || number < 1)
        ui->luckValue->setPalette(redPalette);
    else
        ui->luckValue->setPalette(normPalette);
}
