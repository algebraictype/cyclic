//create a function that takes an argument n and maps it to n + 3
g_n |-> s^n * s^3

//alias this above function call with a value s^m
s^m |-> g_4 * (x_k |-> x^4 * x^5) //notice that because the inner function evaluates to ac onstant, this is valid.

h_m |-> g_4 * s^m (g evaluated at 4 and the alias s^m)
x_k |-> h_m * g_3
