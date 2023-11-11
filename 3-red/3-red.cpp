#include <iostream>
#include <stdexcept>
#include <utility>

using namespace std;

template <typename tip> class Red {
private:
  struct Cvor {
    tip element;
    Cvor *sljedeci;
    Cvor(const tip &element, Cvor *sljedeci = nullptr)
        : element(element), sljedeci(sljedeci) {}
  };
  Cvor *pocetak;
  Cvor *kraj;
  Cvor *trenutni;
  int brElemenata;

public:
  Red() : pocetak(0), kraj(0), brElemenata(0), trenutni(0) {}
  Red(const Red &q)
      : pocetak(nullptr), kraj(nullptr), brElemenata(0), trenutni(nullptr) {
    Cvor *p(q.pocetak);
    while (p != nullptr) {
      stavi(p->element);
      p = p->sljedeci;
    }
  }

  ~Red() { brisi(); }

  Red &operator=(const Red &q) {
    if (&q == this)
      return *this;

    brisi();

    Cvor *p(q.pocetak);
    while (p != nullptr) {
      stavi(p->element);
      p = p->sljedeci;
    }

    return *this;
  }
  int brojElemenata() { return brElemenata; }
  void brisi() {
    while (pocetak != nullptr) {
      Cvor *temp = pocetak;
      pocetak = pocetak->sljedeci;
      delete temp;
    }
    kraj = nullptr;
    brElemenata = 0;
  }

  void stavi(const tip &el) {
    Cvor *n = new Cvor(el);
    if (pocetak == 0)
      pocetak = kraj = n;
    else {
      kraj->sljedeci = n;
      kraj = kraj->sljedeci;
    }
    brElemenata++;
  }
  tip skini() {
    if (brElemenata == 0)
      throw std::domain_error("Prazan red");

    tip info(pocetak->element);
    Cvor *p = pocetak;
    if (pocetak == kraj)
      pocetak = kraj = 0;
    else
      pocetak = pocetak->sljedeci;
    delete p;
    brElemenata--;
    return info;
  }
  tip &celo() {
    if (brElemenata == 0)
      throw std::domain_error("Prazan red");

    return pocetak->element;
  }
};

int main() {

  Red<int> r;
  for (int i = 1; i <= 5; i++)
    r.stavi(i);
  cout << "Broj elemenata: " << r.brojElemenata() << endl;
  cout << "Element na čelu reda: " << r.celo() << endl;
  while (r.brojElemenata() > 0) {
    cout << "Skini: " << r.skini() << endl;
  }
  cout << "Broj elemenata: " << r.brojElemenata() << endl;
  r.brisi();
  cout << "Broj elemenata: " << r.brojElemenata() << endl;

  return 0;
}