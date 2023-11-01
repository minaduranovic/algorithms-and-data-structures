#include <iostream>
#include <stdexcept>

using namespace std;

template <typename tip> class JednostrukaLista : public Lista<tip> {
private:
  struct Cvor {
    tip element;
    Cvor *sljedeci;
    Cvor(const tip &element, Cvor *sljedeci = nullptr)
        : element(element), sljedeci(sljedeci) {}
  };
  int brElemenata;
  Cvor *pocetni;
  Cvor *krajnji;
  Cvor *tekuci;

public:
  JednostrukaLista() : pocetni(0), krajnji(0), tekuci(0), brElemenata(0) {}
  ~JednostrukaLista() {
    while (pocetni != nullptr) {
      Cvor *temp = pocetni;
      pocetni = pocetni->sljedeci;
      delete temp;
    }
  }

  JednostrukaLista(const JednostrukaLista &n)
      : brElemenata(n.brElemenata) {
    pocetni = nullptr;
    krajnji = nullptr;
    tekuci = nullptr;

    Cvor *drCvor = n.pocetni;
    Cvor *prevCvor = nullptr;
    while (drCvor != nullptr) {
      Cvor *noviCvor = new Cvor(drCvor->element);
      if (prevCvor == nullptr) {
        pocetni = noviCvor;
      } else {
        prevCvor->sljedeci = noviCvor;
      }
      prevCvor = noviCvor;
      if (drCvor == n.tekuci) {
        tekuci = noviCvor;
      }
      drCvor = drCvor->sljedeci;
    }
    krajnji = prevCvor;
  }

  JednostrukaLista &operator=(const JednostrukaLista &n) {
    if (this == &n) {
      return *this;
    }

    while (pocetni != nullptr) {
      Cvor *temp = pocetni;
      pocetni = pocetni->sljedeci;
      delete temp;
    }

    brElemenata =n.brElemenata;

    Cvor *drCvor = n.pocetni;
    Cvor *prevCvor = nullptr;
    while (drCvor != nullptr) {
      Cvor *noviCvor = new Cvor(drCvor->element);
      if (prevCvor == nullptr) {
        pocetni = noviCvor;
      } else {
        prevCvor->sljedeci = noviCvor;
      }
      prevCvor = noviCvor;
      if (drCvor == n.tekuci) {
        tekuci = noviCvor;
      }
      drCvor = drCvor->sljedeci;
    }
    krajnji = prevCvor;

    return *this;
  }

  int brojElemenata() const override { return brElemenata; }

  tip &trenutni() override {
    if (tekuci == nullptr) {
      throw std::runtime_error("Trenutni element ne postoji.");
    }
    return tekuci->element;
  }

  tip trenutni() const override {
    if (tekuci == nullptr) {
      throw std::runtime_error("Trenutni element ne postoji.");
    }
    return tekuci->element;
  }
  bool sljedeci() override {
    if (brElemenata == 0)
      throw std::runtime_error("Prazna lista.");
    if (tekuci == nullptr || tekuci->sljedeci == nullptr) {
      return false;
    }
    tekuci = tekuci->sljedeci;
    return true;
  }

  bool prethodni() override {
    if (brElemenata == 0)
      throw std::runtime_error("Prazna lista.");
    if (tekuci == pocetni) {
      return false;
    }
    Cvor *prethodni = pocetni;
    while (prethodni->sljedeci != tekuci) {
      prethodni = prethodni->sljedeci;
    }
    tekuci = prethodni;
    return true;
  }

  void pocetak() override {
    if (brElemenata == 0)
      throw std::runtime_error("Prazna lista.");
    tekuci = pocetni;
  }

  void kraj() override {
    if (brElemenata == 0)
      throw std::runtime_error("Prazna lista.");
    tekuci = krajnji;
  }

  void obrisi() override {
    if (brElemenata == 0)
      throw std::runtime_error("Prazna lista.");

    if (tekuci == pocetni) {
      pocetni = pocetni->sljedeci;
      delete tekuci;
      tekuci = pocetni;
      brElemenata--;
      if (brElemenata == 0)
        krajnji = nullptr;
    } else {
      Cvor *temp(pocetni);
      while (temp->sljedeci != tekuci)
        temp = temp->sljedeci;

      temp->sljedeci = tekuci->sljedeci;
      delete tekuci;
      tekuci = temp->sljedeci;

      if (tekuci == nullptr)
        krajnji = tekuci = temp;

      brElemenata--;
    }
  }

  void dodajIspred(const tip &el) override {
    Cvor *novi = new Cvor(el);
    if (pocetni == nullptr) {
      pocetni = novi;
      krajnji = novi;
      tekuci = novi;
    } else {
      novi->sljedeci = tekuci;
      if (tekuci == pocetni) {
        pocetni = novi;
      } else {
        Cvor *prethodniCvor = pocetni;
        while (prethodniCvor->sljedeci != tekuci) {
          prethodniCvor = prethodniCvor->sljedeci;
        }
        prethodniCvor->sljedeci = novi;
      }
    }
    brElemenata++;
  }

  void dodajIza(const tip &el) override {
    Cvor *novi = new Cvor(el);
    if (pocetni == nullptr) {
      pocetni = novi;
      krajnji = novi;
      tekuci = novi;
    } else {
      novi->sljedeci = tekuci->sljedeci;
      tekuci->sljedeci = novi;
      if (tekuci == krajnji) {
        krajnji = novi;
      }
    }
    brElemenata++;
  }

  tip &operator[](int i) override {
    if (i < 0 || i >= brElemenata) {
      throw std::out_of_range("Indeks izvan granica.");
    }
    Cvor *temp = pocetni;
    for (int j = 0; j < i; j++) {
      if (temp == nullptr) {
        throw std::out_of_range("Indeks izvan granica.");
      }
      temp = temp->sljedeci;
    }
    return temp->element;
  }

  tip operator[](int i) const override {
    if (i < 0 || i >= brElemenata) {
      throw std::out_of_range("Indeks izvan granica.");
    }
    Cvor *temp = pocetni;
    for (int j = 0; j < i; j++) {
      if (temp == nullptr) {
        throw std::out_of_range("Indeks izvan granica.");
      }
      temp = temp->sljedeci;
    }
    return temp->element;
  }
};

int main() {

  JednostrukaLista<string> jednostrukaLista;
  cout << "Testiranje JednostrukaLista:" << endl;
  jednostrukaLista.dodajIspred("A");
  jednostrukaLista.dodajIza("B");
  jednostrukaLista.dodajIza("C");
  cout << "Broj elemenata: " << jednostrukaLista.brojElemenata() << endl;
  jednostrukaLista.pocetak();
  cout << "Trenutni element: " << jednostrukaLista.trenutni() << endl;
  jednostrukaLista.sljedeci();
  cout << "Sljedeci element: " << jednostrukaLista.trenutni() << endl;
  jednostrukaLista.prethodni();
  cout << "Prethodni element: " << jednostrukaLista.trenutni() << endl;
  jednostrukaLista.kraj();
  cout << "Krajnji element: " << jednostrukaLista.trenutni() << endl;
  jednostrukaLista.obrisi();
  cout << "Broj elemenata nakon brisanja: " << jednostrukaLista.brojElemenata()
       << endl;
  jednostrukaLista.dodajIspred("D");
  cout << "Dodan novi element ispred: " << jednostrukaLista.trenutni() << endl;
  jednostrukaLista.dodajIza("E");
  cout << "Dodan novi element iza: " << jednostrukaLista.trenutni() << endl;
  cout << "indeks 1: " << jednostrukaLista[1] << endl;

  return 0;
}