#include <iostream>
#include <stdexcept>

using namespace std;

template <typename tip> class Lista {
public:
  Lista() {}
  virtual ~Lista() {}
  virtual int brojElemenata() const = 0;
  virtual tip &trenutni() = 0;
  virtual tip trenutni() const = 0;
  virtual bool sljedeci() = 0;
  virtual bool prethodni() = 0;
  virtual void pocetak() = 0;
  virtual void kraj() = 0;
  virtual void obrisi() = 0;
  virtual void dodajIspred(const tip &el) = 0;
  virtual void dodajIza(const tip &el) = 0;
  virtual tip &operator[](int i) = 0;
  virtual tip operator[](int i) const = 0;
};

////////

template <typename tip> class NizLista : public Lista<tip> {
private:
  tip **lniz;
  int kapacitet;
  int brElemenata;
  int tekuci;

  void realociraj() {
    int noviKap = kapacitet * 2;
    tip **noviLniz = new tip *[noviKap];

    for (int i = 0; i < brElemenata; i++) {
      noviLniz[i] = lniz[i];
    }

    delete[] lniz;
    lniz = noviLniz;
    kapacitet = noviKap;
  }

public:
  NizLista() {
    kapacitet = 10;
    brElemenata = 0;
    tekuci = 0;
    lniz = new tip *[kapacitet];
    for (int i = 0; i < kapacitet; i++) {
      lniz[i] = nullptr;
    }
  }

  ~NizLista() {
    for (int i = 0; i < brElemenata; i++) {
      delete lniz[i];
    }
    delete[] lniz;
  }
  int brojElemenata() const override { return brElemenata; }

  NizLista(const NizLista &n) {
    kapacitet = n.kapacitet;
    brElemenata = n.brElemenata;
    tekuci = n.tekuci;

    lniz = new tip *[kapacitet];
    for (int i = 0; i < brElemenata; i++) {
      lniz[i] = new tip(*(n.lniz[i]));
    }
  }
  NizLista &operator=(const NizLista &n) {
    if (this == &n) {
      return *this;
    }
    for (int i = 0; i < brElemenata; i++) {
      delete lniz[i];
    }
    delete[] lniz;

    kapacitet = n.kapacitet;
    brElemenata = n.brElemenata;
    tekuci = n.tekuci;

    lniz = new tip *[kapacitet];
    for (int i = 0; i < brElemenata; i++) {
      lniz[i] = new tip(*(n.lniz[i]));
    }

    return *this;
  }

  tip &trenutni() override {
    if (brElemenata == 0 || tekuci < 0 || tekuci >= brElemenata) {
      throw std::runtime_error("Nema trenutnog elementa.");
    }
    return *(lniz[tekuci]);
  }

  tip trenutni() const override {
    if (brElemenata == 0 || tekuci < 0 || tekuci >= brElemenata) {
      throw std::runtime_error("Nema trenutnog elementa.");
    }
    return *(lniz[tekuci]);
  }

  bool sljedeci() override {
    if (brElemenata == 0)
      throw std::runtime_error("Prazna lista.");
    if (brElemenata == 0 || tekuci >= brElemenata - 1) {
      return false;
    }
    tekuci++;
    return true;
  }

  bool prethodni() override {
    if (brElemenata == 0)
      throw std::runtime_error("Prazna lista.");
    if (brElemenata == 0 || tekuci <= 0) {
      return false;
    }
    tekuci--;
    return true;
  }

  void pocetak() override {
    if (brElemenata == 0)
      throw std::runtime_error("Prazna lista.");
    tekuci = 0;
  }

  void kraj() override {
    if (brElemenata == 0)
      throw std::runtime_error("Prazna lista.");
    if (brElemenata > 0) {
      tekuci = brElemenata - 1;
    }
  }

  void obrisi() override {
    if (brElemenata == 0)
      throw std::runtime_error("Prazna lista.");

    delete lniz[tekuci];

    for (int i = tekuci; i < brElemenata - 1; i++) {
      lniz[i] = lniz[i + 1];
    }

    lniz[brElemenata - 1] = nullptr;

    if (tekuci == brElemenata - 1) {
      prethodni();
    } else {
    }

    brElemenata--;
  }

  void dodajIspred(const tip &el) override {
    if (brElemenata >= kapacitet) {
      realociraj();
    }
    if (lniz[tekuci] == nullptr) {
      lniz[tekuci] = new tip(el);
      brElemenata++;

    } else {
      for (int i = brElemenata; i > tekuci; i--) {
        lniz[i] = lniz[i - 1];
      }

      lniz[tekuci] = new tip(el);
      tekuci++;

      brElemenata++;
    }
  }

  void dodajIza(const tip &el) override {
    if (brElemenata >= kapacitet) {
      realociraj();
    }
    if (lniz[tekuci] == nullptr) {
      lniz[tekuci] = new tip(el);
      brElemenata++;
    } else {
      for (int i = brElemenata; i > tekuci; i--) {
        lniz[i] = lniz[i - 1];
      }
      lniz[tekuci + 1] = new tip(el);

      brElemenata++;
    }
  }

  tip &operator[](int i) override {
    if (i < 0 || i >= brElemenata) {
      throw std::out_of_range("Indeks van opsega.");
    }
    return *(lniz[i]);
  }
  tip operator[](int i) const override {
    if (i < 0 || i >= brElemenata) {
      throw std::out_of_range("Indeks van opsega.");
    }
    return *(lniz[i]);
  }
};
int main() {

  NizLista<int> nizLista;
  cout << "Testiranje NizLista:" << endl;
  nizLista.dodajIspred(1);
  nizLista.dodajIza(2);
  nizLista.dodajIza(3);
  cout << "Broj elemenata: " << nizLista.brojElemenata() << endl;
  nizLista.pocetak();
  cout << "Trenutni element: " << nizLista.trenutni() << endl;
  nizLista.sljedeci();
  cout << "Sljedeci element: " << nizLista.trenutni() << endl;
  nizLista.prethodni();
  cout << "Prethodni element: " << nizLista.trenutni() << endl;
  nizLista.kraj();
  cout << "Krajnji element: " << nizLista.trenutni() << endl;
  nizLista.obrisi();
  cout << "Broj elemenata nakon brisanja: " << nizLista.brojElemenata() << endl;
  nizLista.dodajIspred(4);
  cout << "Dodan novi element ispred: " << nizLista.trenutni() << endl;
  nizLista.dodajIza(5);
  cout << "Dodan novi element iza: " << nizLista.trenutni() << endl;
  cout << "indeks 1: " << nizLista[1] << endl;


  return 0;
}
