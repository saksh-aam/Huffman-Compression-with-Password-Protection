#include <bits/stdc++.h>
#define ll long long int
inline ll power(ll x, unsigned ll y, ll p = LLONG_MAX)
{
    ll res = 1;
    x = x % p;
    if (x == 0)
    {
        return 0;
    }
    while (y > 0)
    {
        if (y & 1)
        {
            res = (res * x) % p;
        }
        y = y >> 1;
        x = (x * x) % p;
    }
    return res;
}

class RSA
{
public:
    ll p, q, n, phi_n, e, d;
    ll cipher, plain;

    RSA(ll p = 0, ll q = 0)
    {
        this->p = p;
        this->q = q;
        if (p == 0 and q == 0)
        {
            generatePublicKey();
            calcuate_n();
        }
    }

    bool isPrime(int n)
    {
        for (int i = 2; i <= n / i; ++i)
        {
            if (n % i == 0)
            {
                return false;
            }
        }
        return true;
    }
    void generatePublicKey()
    {
        ll upperlimit = 9999, lowerlimit = 999;
        ll range = upperlimit - lowerlimit + 1;

        srand(time(NULL));
        do
        {
            p = (rand() % range + lowerlimit);
        } while (!isPrime(p));
        do
        {
            q = (rand() % range + lowerlimit);
        } while (!isPrime(q));
    }

    void calcuate_n()
    {
        n = p * q;
        phi_n = (p - 1) * (q - 1);
    }
    void calculate_e()
    {
        calcuate_n();
        ll i = 2;
        while (__gcd(i, phi_n) != 1)
            i++;

        e = i;
        if (e >= phi_n)
        {
            throw(runtime_error("Encryption gone wrong!!"));
        }
    }
    void calculate_d()
    {
        calculate_e();
        ll k = 1;
        while (((k * phi_n) + 1) % e != 0)
            k++;

        d = ((k * phi_n) + 1) / e;
    }

    void encrypt(ll plainnumber)
    {
        calculate_e();
        cipher = power(plainnumber, e, n);
    }
    void decrypt(ll ciphernumber)
    {
        calculate_d();
        plain = power(ciphernumber, d, n);
    }
};