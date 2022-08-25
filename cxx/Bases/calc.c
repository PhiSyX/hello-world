#include <stdbool.h>
#include <stdio.h>

double
calc(double operand1, double operand2, char operator);

int
main(void)
{
  calc(1.0, 2.0, '+');
  calc(10.0, 7.0, '-');
  calc(4.0, 2.3, '*');
  calc(5.0, 0.0, '/');
  calc(5.0, 2.0, '%');
  calc(1.0, 2.0, '?');

  return 0;
}

double
calc(double op1, double op2, char operator)
{
  double result = 0.0;

  printf("%g %c %g = ", op1, operator, op2);

  switch (operator) {
    case '+':
      result = op1 + op2;
      break;

    case '-':
      result = op1 - op2;
      break;

    case '*':
      result = op1 * op2;
      break;

    case '/':
      if (op2 == 0.0) {
        printf("Erreur: division par %g\n", op2);
        return result;
      } else {
        result = op1 / op2;
      }
      break;

    case '%':
      result = (int)op1 % (int)op2;
      break;

    default:
      printf("Erreur: opérateur attendue: + - * / ou %%\n");
      return result;
      break;
  }

  printf("%g\n", result);

  return result;
}