/*!
 * \file main.h
 * \brief Заголовочный файл с описанием функций
 *
 * Данный файл содержит в себе определения основных
 * функций, используемых в программе.
*/

#ifndef INC_2_LAB_MAIN_H
#define INC_2_LAB_MAIN_H

/*!
 * \brief Считывает из консоли одно целое число
 * \param[in] message Выводимое сообщение
 * \param[in] lowerBound Нижняя граница чисел
 * \param[in] count Верхняя граница чисел
 * \return Считанное число
 */
int CheckingInput(const char message[], int lowerBound, int count);

/*!
 * \brief Функция, которую будет выполнять процесс
 * \param[in] arg Аргументы, переданные в функцию
 */
void* train(void* arg);

/*!
 * \brief Точка входа в программу
 * \return Код завершения
 */
int main();

#endif //INC_2_LAB_MAIN_H
