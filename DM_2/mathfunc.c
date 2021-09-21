#include "Header.h"

//����������
uint U(uint m, uint n, error_t* err) {
  if (m == 0)
    return 0;
  else if (n == 0 || m == 1)
    return 1;

  uint i, ans = m;
  for (i = 1; i < n; i++) {

    //���������� 32 ���
    if (((double)UINT_MAX) / m < ans) {
      *err = TOO_LARGE_NUM;
      return 0;
    }

    ans *= m;
  }
  return ans;
}

//���������� ��� ����������
uint A(uint m, uint n, error_t* err) {

  if (n > m)
    return 0;
  if (n == 0)
    return 1;

  uint i = 0, ans = 1;

  for (i = 0; i <= (n - 1); i++) {

    //���������� 32 ���
    if (((double)UINT_MAX) / (m - i) < ans) {
      *err = TOO_LARGE_NUM;
      return 0;
    }

    ans *= (m - i);
  }

  return ans;
}

//������������
uint P(uint n, error_t* err) {

  //12! ����� ����������� 32 ������, 13! � ������ ������
  if (n > 12) {
    *err = TOO_LARGE_NUM;
    return 0;
  }

  if (n == 0 || n == 1)
    return 1;

  uint ans = 1, i = 1;

  for (i = 1; i <= n; i++)
    ans *= i;

  return ans;
}

//���������
uint C(uint m, uint n, error_t* err) {

  if (n == 1 || n == (m - 1))
    return m;
  else if (n == 0 || n == m)
    return 1;
  else if (n > m)
    return 0;

  if (n > m - n)
    n = m - n; //� ���� ���������
  uint i, j, ans;
  uint* A = (uint*)malloc((n + 1) * sizeof(uint));
  if (!A) {
    *err = NOT_ENOUGH_MEMORY;
    return 0;
  }

  for (i = 0; i <= n; i++)
    A[i] = 1;

  for (i = 1; i <= m - n; i++) {
    for (j = 1; j <= n; j++) {

      //���������� 32 ���
      if (A[j] > UINT_MAX - A[j - 1]) {
        free(A);
        *err = TOO_LARGE_NUM;
        return 0;
      }

      A[j] += A[j - 1];
    }
  }
  ans = A[n];
  free(A);
  return ans;
}

//����� ��������� ������� ����
uint S(uint m, uint n, error_t* err) {
  if (n == m || n == 1)
    return 1;
  else if (n == 0 || n > m)
    return 0;

  uint i, j, ans;
  uint d = MIN(n, m - n + 1), s = MAX(n, m - n + 1);

  uint* D = (uint*)malloc(d * sizeof(uint));
  if (!D) {
    *err = NOT_ENOUGH_MEMORY;
    return 0; // ������
  }

  for (i = 0; i < d; i++)
    D[i] = 1;

  for (i = 2; i <= s; i++) {
    for (j = 2; j <= d; j++) {

      if (d == n) {
        //���������� 32 ���
        if ( D[j-1] > ((double)UINT_MAX)/j || D[j-2] > UINT_MAX - j * D[j-1]) {
          free(D);
          *err = TOO_LARGE_NUM;
          return 0;
        }

        D[j - 1] = D[j - 2] + j * D[j - 1];
      }
      else {
        //���������� 32 ���
        if (D[j-2] > ((double)UINT_MAX)/i || D[j-1] > UINT_MAX - i * D[j-2]) {
          free(D);
          *err = TOO_LARGE_NUM;
          return 0;
        }
        D[j - 1] = D[j - 1] + i * D[j - 2];
      }
    }
  }
  ans = D[d - 1];
  free(D);
  return ans;
}

//����� ����� 
uint B(uint m, error_t* err) {
  if (m == 0)
    return 1;

  //B(15) ����������� 32 ������, �(16) ��� ���
  else if (m > 15) {
    *err = TOO_LARGE_NUM;
    return 0;
  }

  uint t, s, ans; //�������� ��������
  uint n, k; //�������� �����
  uint* A = (uint*)malloc(m * sizeof(uint));
  if (!A) {
    *err = NOT_ENOUGH_MEMORY;
    return 0;
  }

  A[0] = 1;
  for (n = 1; n < m; n++) {
    t = A[0];
    A[0] = A[n - 1];

    for (k = 1; k <= n; k++) {
      s = A[k];
      A[k] = A[k - 1] + t;
      t = s;
    }
  }
  ans = A[m - 1];
  free(A);
  return ans;
}