#include <iostream>

using namespace std;

template <typename tipKljuca, typename tipVrijednosti> class Mapa {
public:
  virtual ~Mapa() {}
  virtual tipVrijednosti &operator[](const tipKljuca &kljuc) = 0;
  virtual tipVrijednosti operator[](const tipKljuca &kljuc) const = 0;
  virtual int brojElemenata() const = 0;
  virtual void obrisi() = 0;
  virtual void obrisi(const tipKljuca &kljuc) = 0;
};

template <typename TipKljuca, typename TipVrijednosti>
class BinStabloMapa : public Mapa<TipKljuca, TipVrijednosti> {
  struct Cvor {
    TipKljuca kljuc;
    TipVrijednosti vrijednost;
    Cvor *lijevo; 
    Cvor *desno; 
    Cvor *roditelj;

    Cvor(const TipKljuca &k, const TipVrijednosti &v, Cvor *l, Cvor *d, Cvor *r)
        : kljuc(k), vrijednost(v), lijevo(l), desno(d), roditelj(r) {}
  };

  Cvor *korijen;
  int brojEl;
  TipVrijednosti prazan;

  void obrisiSve(Cvor *p) {
    if (p == nullptr) return;
    obrisiSve(p->lijevo);
    obrisiSve(p->desno);
    delete p;
    p = nullptr;
    brojEl--;
  }

 void kopiraj(Cvor *&trenutni, Cvor *other, Cvor *roditelj) {
    if (other == nullptr)  return;
    trenutni = new Cvor(other->kljuc, other->vrijednost, nullptr, nullptr, roditelj);
    kopiraj(trenutni->lijevo, other->lijevo, trenutni);
    kopiraj(trenutni->desno, other->desno, trenutni);
    brojEl++;
  }

public:

 ~BinStabloMapa() {
    obrisiSve(korijen);
    brojEl = 0;
  }
  BinStabloMapa() : brojEl(0), korijen(nullptr), prazan(TipVrijednosti()) {}


  BinStabloMapa(const BinStabloMapa<TipKljuca, TipVrijednosti> &b)  : korijen(0), brojEl(0), prazan(TipVrijednosti()) {
    kopiraj(korijen, b.korijen, nullptr);
  }


BinStabloMapa operator=(const BinStabloMapa<TipKljuca, TipVrijednosti> &b) {
    if (this != &b) {
        obrisiSve(korijen);
        brojEl = 0;
        kopiraj(korijen, b.korijen, nullptr);
    }
    return *this;
}
  int brojElemenata() const { return brojEl; }

  void obrisi() {
    obrisiSve(korijen);
    brojEl = 0;
    korijen = nullptr;
  }

  void obrisi(const TipKljuca &k) {
    Cvor *p = korijen;
    while (p != nullptr) {
      if (p->kljuc < k){
        p = p->desno;
      }
      else if (p->kljuc > k){
        p = p->lijevo;
      }
      else if (p->kljuc == k) {
        if (p->lijevo == nullptr && p->desno == nullptr) { 
          if (p != korijen) {
            if (p->roditelj->lijevo == p) 
              p->roditelj->lijevo = nullptr;
            else
              p->roditelj->desno = nullptr;
            delete p;
            p= nullptr;
            brojEl--;
          } else if (p == korijen) {
            delete korijen;
            korijen = nullptr;
            brojEl--;
          }
        } else if (p->lijevo != nullptr && p->desno != nullptr) {
          Cvor *z = p->lijevo;
          Cvor *pz = nullptr;
          while (z->desno != nullptr) {
            pz = z;
            z = z->desno;
          }
          p->kljuc = z->kljuc;
          p->vrijednost = z->vrijednost;
          if (pz != nullptr)
            z->roditelj->desno = z->lijevo;
          else
            z->roditelj->lijevo = z->lijevo;
          delete z;
          z = nullptr;
          brojEl--;
        } else {
          Cvor *tmp = p->lijevo;
          if (tmp == nullptr)
            tmp = p->desno;
          if (p->roditelj == nullptr) {
            korijen = tmp;
            korijen->roditelj = nullptr;
          } else {
            if (p->roditelj->lijevo == p)
              p->roditelj->lijevo = tmp;
            else
              p->roditelj->desno = tmp;
            tmp->roditelj = p->roditelj;
          }
          delete p;
          p = nullptr;
          brojEl--;
        }

      } else
        throw std::out_of_range("Kljuc nije pronadjen!");
    }
  }

  TipVrijednosti &operator[](const TipKljuca &k) {
    Cvor *p1 = nullptr;
    Cvor *p2 = nullptr;
    if (brojEl != 0) {
      p1 = korijen;
      while (p1 != nullptr) {
        p2 = p1;
        if (p1->kljuc < k)
          p1 = p1->desno;
        else if (p1->kljuc > k)
          p1 = p1->lijevo;
        else if (p1->kljuc == k)
          return p1->vrijednost;

      }

      Cvor *temp = new Cvor(k, TipVrijednosti(), nullptr, nullptr, nullptr);

      if (p2->kljuc < k) {
        p2->desno = temp;
        temp->roditelj = p2;
      } else {
        p2->lijevo = temp;
        temp->roditelj = p2;
      }

      brojEl++;
      return temp->vrijednost;
    } else {
      korijen = new Cvor(k, TipVrijednosti(), nullptr, nullptr, nullptr);
      brojEl++;
      return korijen->vrijednost = TipVrijednosti();
    }
  }

  TipVrijednosti operator[](const TipKljuca &k) const { 
    Cvor *p = korijen;
    while (p!= nullptr) {
      if (p->kljuc < k)
        p = p->desno;
      else if (p->kljuc > k)
       p = p->lijevo;
      else if (p->kljuc == k)
        return p->vrijednost;
    }
   static TipVrijednosti t = TipVrijednosti();
   return t;
  }
 
};

template <typename tipKljuca, typename tipVrijednosti>
class NizMapa : public Mapa<tipKljuca, tipVrijednosti> {
  std::pair<tipKljuca, tipVrijednosti> **elementi;
  int kapacitet;
  int velicina;

public:
  NizMapa() : kapacitet(10), velicina(0) {
    elementi = new std::pair<tipKljuca, tipVrijednosti> *[kapacitet];
    for (int i = 0; i < kapacitet; i++) {
      elementi[i] = nullptr;
    }
  }

  ~NizMapa() {
    obrisi();
    delete[] elementi;
  }
  NizMapa(const NizMapa &other)
      : kapacitet(other.kapacitet), velicina(other.velicina) {
    elementi = new std::pair<tipKljuca, tipVrijednosti> *[kapacitet];
    for (int i = 0; i < velicina; i++) {
      elementi[i] =
          new std::pair<tipKljuca, tipVrijednosti>(*(other.elementi[i]));
    }
    for (int i = velicina; i < kapacitet; i++) {
      elementi[i] = nullptr;
    }
  }

  NizMapa &operator=(const NizMapa &other) {
    if (this != &other) {
      obrisi();
      delete[] elementi;

      kapacitet = other.kapacitet;
      velicina = other.velicina;

      elementi = new std::pair<tipKljuca, tipVrijednosti> *[kapacitet];
      for (int i = 0; i < velicina; i++) {
        elementi[i] =
            new std::pair<tipKljuca, tipVrijednosti>(*(other.elementi[i]));
      }
      for (int i = velicina; i < kapacitet; i++) {
        elementi[i] = nullptr;
      }
    }
    return *this;
  }

  tipVrijednosti &operator[](const tipKljuca &kljuc) override {
    for (int i = 0; i < velicina; i++) {
      if (elementi[i]->first == kljuc) {
        return elementi[i]->second;
      }
    }
    if (velicina == kapacitet) {
      realociraj();
    }
    elementi[velicina++] =
        new std::pair<tipKljuca, tipVrijednosti>(kljuc, tipVrijednosti());
    return elementi[velicina - 1]->second;
  }

  tipVrijednosti operator[](const tipKljuca &kljuc) const override {
    for (int i = 0; i < velicina; i++) {
      if (elementi[i]->first == kljuc) {
        return elementi[i]->second;
      }
    }

    static tipVrijednosti t = tipVrijednosti();
    return t;
  }

  int brojElemenata() const override { return velicina; }

  void obrisi() override {
    for (int i = 0; i < velicina; i++) {
      delete elementi[i];
    }
    velicina = 0;
  }

  void obrisi(const tipKljuca &kljuc) override {
    for (int i = 0; i < velicina; i++) {
      if (elementi[i]->first == kljuc) {
        delete elementi[i];
        for (int j = i; j < velicina - 1; j++) {
          elementi[j] = elementi[j + 1];
        }
        velicina--;
        return;
      }
    }
    throw std::out_of_range("Kljuc nije pronadjen!");
  }

private:
  void realociraj() {
    kapacitet *= 2;
    std::pair<tipKljuca, tipVrijednosti> **noviElementi =
        new std::pair<tipKljuca, tipVrijednosti> *[kapacitet];
    for (int i = 0; i < kapacitet; i++) {
      noviElementi[i] = nullptr;
    }
    for (int i = 0; i < velicina; i++) {
      noviElementi[i] = elementi[i];
    }
    delete[] elementi;
    elementi = noviElementi;
  }
};

int main() {
  BinStabloMapa<int, int> binarno;
  NizMapa<int, int> niz;

  clock_t t1 = clock();
  for (int i = 0; i < 10000; i++){
    binarno[i] = i;
  }
  clock_t t2 = clock();
  int ukupno1 = (t2 - t1) / (CLOCKS_PER_SEC / 1000);
  std::cout << "Vrijeme za binarno stablo mapu je:" << ukupno1 << " ms." <<std::endl;
  clock_t t3 = clock();
  for (int i = 0; i < 10000; i++){
    niz[i] = i;
  }
  clock_t t4 = clock();
  int ukupno2 = (t4 - t3) / (CLOCKS_PER_SEC / 1000);
  std::cout << "Vrijeme za niz mapu je:" << ukupno2
       << " ms." << std::endl;

  // zakljucak je da je dodavanje elemenata efikasnije kod binarnog stabla mape

  t1 = clock();
  binarno[5500];
  binarno[1234];
  binarno[9];
  t2 = clock();
  ukupno1 = (t4 - t3) / (CLOCKS_PER_SEC / 1000);
  cout << "Vrijeme pristupa elemenata u binarnom stablu mapi je:"
       << ukupno1 << " ms." << endl;
 t3 = clock();
  niz[5500];
  niz[1234];
  niz[9];
  t4 = clock();
  ukupno2 = (t4 - t3) / (CLOCKS_PER_SEC / 1000);
  std::cout << "Vrijeme pristupa elemenata u niz mapi je:" << ukupno2
       << " ms." << endl;

//zakljucak je da je pristup elementima efikasniji kod niz mape

  return 0;
}
