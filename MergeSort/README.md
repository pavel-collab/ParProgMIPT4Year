Сначала прогоняем сортировку на разных процессах и с разной глубиной рекурсии, таким образом,
ищем оптимальную пару (количество процессов, глубина рекурсии).

Затем на заанной глубине рекурсии строим графики ускорения и эффективности.

Заметим, что для пузырьковой и быстрой сортировки оптимальная глубина рекурсии может отличаться.

В ходе предварительных эксперементов на массиве размером 1048576 элементов было установлено:
- при использовании quick сортировки оптимальное соотношение потоки/глубина рекурсии = 2/291
- при использовании bubble сортировки оптимальное соотношение потоки/глубина рекурсии = 2/296