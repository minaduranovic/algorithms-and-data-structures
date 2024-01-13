#include <iostream>
#include <stdexcept>
#include <vector>
#include <chrono>
#include <random>
using namespace std;

unsigned int djbhash(int ulaz, unsigned int max) {
  unsigned int suma = 5381;
  std::string ulazString = std::to_string(ulaz);
  for (char c : ulazString)
    suma = suma * 33 + static_cast<unsigned int>(c);
  return suma % max;
}
template <typename tipKljuca, typename tipVrijednosti> class Mapa {
public:
  virtual ~Mapa() {}
  virtual tipVrijednosti &operator[](const tipKljuca &kljuc) = 0;
  virtual const tipVrijednosti operator[](tipKljuca kljuc) const = 0;
  virtual int brojElemenata() const = 0;
  virtual void obrisi() = 0;
  virtual void obrisi(const tipKljuca &kljuc) = 0;
};

template <typename TipKljuca, typename TipVrijednosti>
class HashMapa : public Mapa<TipKljuca, TipVrijednosti> {
protected:
  int kapacitet;
  int brEl;
  std::pair<TipKljuca, TipVrijednosti> **niz;
  bool *prazno;
  unsigned int (*hashFunkcija)(TipKljuca, unsigned int);

public:
  HashMapa()
      : Mapa<TipKljuca, TipVrijednosti>(),
        niz(new std::pair<TipKljuca, TipVrijednosti> *[100000]()),
        kapacitet(100000), brEl(0), prazno(new bool[100000]),
        hashFunkcija(nullptr) {

    for (int i = 0; i < 100000; i++) {
      prazno[i] = false;
    }
  }

  ~HashMapa() {
    if (niz != nullptr) {
      for (int i = 0; i < kapacitet; i++) {
        delete niz[i];
      }
      delete[] niz;
    }

    if (prazno)
      delete[] prazno;
  }

  HashMapa(const HashMapa<TipKljuca, TipVrijednosti> &other) {
    brEl = other.brEl;
    kapacitet = other.kapacitet;
    hashFunkcija = other.hashFunkcija;
    prazno = new bool[kapacitet];

    if (other.kapacitet == 0) {
      niz = nullptr;
      for (int i = 0; i < kapacitet; i++) {
        prazno[i] = false;
      }
    } else {
      niz = new std::pair<TipKljuca, TipVrijednosti> *[kapacitet]();
      for (int i = 0; i < other.kapacitet; i++) {
        if (other.niz[i]) {
          niz[i] = new std::pair<TipKljuca, TipVrijednosti>(*other.niz[i]);
        }
        prazno[i] = other.prazno[i];
      }
    }
  }

  HashMapa<TipKljuca, TipVrijednosti> &
  operator=(const HashMapa<TipKljuca, TipVrijednosti> &other) {

    if (this == &other)
      return *this;

    if (other.kapacitet > kapacitet)
      prosiri(other.kapacitet);

    for (int i = 0; i < kapacitet; i++) {
      prazno[i] = false;
    }

    brEl = other.brEl;
    hashFunkcija = other.hashFunkcija;

    for (int i = 0; i < other.kapacitet; i++) {
      if (other.niz[i]) {
        delete niz[i];
        niz[i] = new std::pair<TipKljuca, TipVrijednosti>(*other.niz[i]);
      }
      prazno[i] = other.prazno[i];
    }

    return *this;
  }

  void prosiri(int dodatno = 10000) {
    std::pair<TipKljuca, TipVrijednosti> **niz2 =
        new std::pair<TipKljuca, TipVrijednosti> *[dodatno + kapacitet]();
    bool *q = new bool[dodatno + kapacitet]();

    for (int i = 0; i < kapacitet; i++) {
      if (niz[i] != nullptr)
        niz2[i] = new std::pair<TipKljuca, TipVrijednosti>(*niz[i]);
      delete niz[i];
      q[i] = prazno[i];
    }
    for (int i = 0; i < dodatno; i++) {
      q[kapacitet + i] = false;
    }

    delete[] niz;
    delete[] prazno;
    kapacitet += dodatno;
    niz = niz2;
    prazno = q;
  }

  void obrisi() override {
    if (brEl == 0)
      throw std::domain_error("Nema elemenata");

    for (int i = 0; i < kapacitet; i++)
      prazno[i] = false;

    brEl = 0;
  }

  void obrisi(const TipKljuca &kljuc) override {
    for (int i = 0; i < kapacitet; i++)
      if (niz[i] != nullptr && niz[i]->first == kljuc) {
        prazno[i] = false;
        brEl--;
        return;
      }

    throw std::domain_error("Nema kljuca");
  }

  int brojElemenata() const override { return brEl; }
  int Velicina() const { return kapacitet; }

  bool UmetniHash(const TipKljuca &k) {
    int j, i = 1;
    do {
      do {
        j = hashFunkcija(k, i);
        if (prazno[j] == false) {
          delete niz[j];
          niz[j] =
              new std::pair<TipKljuca, TipVrijednosti>(k, TipVrijednosti());
          prazno[j] = true;
          return true;
        } else
          i = i + 1;
      } while (i < kapacitet);

      prosiri();
    } while (kapacitet < 100000000);

    return false;
  }

  int TraziHash(const TipKljuca &k) const {
    int indeks, i = 1;
    do {
      indeks = hashFunkcija(k, i);
      if (prazno[indeks] && niz[indeks]->first == k)
        return indeks;
      else
        i = i + 1;
    } while ((prazno[indeks] == true && i != kapacitet) ||
             prazno[hashFunkcija(k, i)] == true);

    return -1;
  }

  TipVrijednosti &operator[](const TipKljuca &kljuc) override {
    if (!hashFunkcija)
      throw std::domain_error("Hash funkcija nije definisana");

    if (brEl == kapacitet)
      prosiri();

    int in = hashFunkcija(kljuc, kapacitet);
    if (!prazno[in]) {
      delete niz[in];
      niz[in] =
          new std::pair<TipKljuca, TipVrijednosti>(kljuc, TipVrijednosti());
      brEl++;
      prazno[in] = true;
      return niz[in]->second;
    }
    if (prazno[in] && niz[in]->first == kljuc)
      return niz[in]->second;
    else {
      in = TraziHash(kljuc);
      if (in == -1) {
        UmetniHash(kljuc);
        brEl++;
        in = TraziHash(kljuc);
        prazno[in] = true;
      }
    }

    return niz[in]->second;
  }

  const TipVrijednosti operator[](TipKljuca kljuc) const override {
    if (!hashFunkcija)
      throw std::domain_error("Hash funkcija nije definisana");

    int in = hashFunkcija(kljuc, kapacitet);
    if (prazno[in] && niz[in]->first == kljuc)
      return niz[in]->second;
    else {
      in = TraziHash(kljuc);
      if (in == -1)
        return TipVrijednosti();
    }

    return niz[in]->second;
  }

  void definisiHashFunkciju(unsigned int (*funkcija)(TipKljuca, unsigned int)) {
    hashFunkcija = funkcija;
    return;
  }
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

         Cvor(TipKljuca k = TipKljuca(), TipVrijednosti v = TipVrijednosti())
        : kljuc(k), vrijednost(v), desno(nullptr), lijevo(nullptr),
          roditelj(nullptr) {}
  };

  Cvor *korijen;
  int brojEl;
  TipVrijednosti del;

   Cvor *pretraga(Cvor *p, TipKljuca t) const {
      if (p == nullptr || p->kljuc == t)
        return p;
      if (p->kljuc < t)
        return pretraga(p->desno, t);
      else if (p->kljuc > t)
        return pretraga(p->lijevo, t);
      return p;
    }



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

    if (trenutni->lijevo != nullptr) trenutni->lijevo->roditelj = trenutni;

    if (trenutni->desno != nullptr) trenutni->desno->roditelj = trenutni;
    brojEl++;
}

 Cvor *dodaj(Cvor *p, TipKljuca k, Cvor* rod=nullptr) {
      if (p == nullptr) {
        p=new Cvor();
         p->kljuc = k;
       p->vrijednost = del;
       p->desno = p->lijevo = p->roditelj = nullptr;
        p->roditelj = rod;
        return p;
      }
      if (k > p->kljuc)
        p->desno = dodaj(p->desno, k,  p);
      else
        p->lijevo = dodaj(p->lijevo, k,  p);
      return p;
    }

public:

~BinStabloMapa() {
    obrisiSve(korijen);
    brojEl = 0;
    korijen = nullptr;  
}

  BinStabloMapa() : brojEl(0), korijen(nullptr), del(TipVrijednosti()) {}


  BinStabloMapa(const BinStabloMapa<TipKljuca, TipVrijednosti> &b)  : korijen(0), brojEl(0), del(TipVrijednosti()) {
    kopiraj(korijen, b.korijen, nullptr);
  }


BinStabloMapa &operator=(const BinStabloMapa<TipKljuca, TipVrijednosti> &other) {
    if (this != &other) {
        obrisiSve(korijen);
        brojEl = 0;
        kopiraj(korijen, other.korijen, nullptr);
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
                p = nullptr;
                brojEl--;
                break;  
            } else if (p == korijen) {
                delete korijen;
                korijen = nullptr;
                brojEl--;
                break;  
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
    
     if (korijen == nullptr) {
      korijen = new Cvor();
      korijen->kljuc=k;
      korijen->vrijednost=del;
      brojEl++;
      return korijen->vrijednost;
    }

    Cvor* c = pretraga(korijen, k);

    if (c == nullptr) {
      dodaj(korijen, k);
      brojEl++;
      c = pretraga(korijen, k);
    }

    return c->vrijednost;
  }

 const TipVrijednosti operator[](TipKljuca k) const { 
 Cvor *p = korijen;
     Cvor* vr = pretraga(p, k);
    if (vr != nullptr)
      return vr->vrijednost;
    return TipVrijednosti(); 
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

 const tipVrijednosti operator[]( tipKljuca kljuc) const override {
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

  NizMapa<int, std::string> nizMapa;
  BinStabloMapa<int, std::string> binStabloMapa;
  HashMapa<int, std::string> hashMapa;


  hashMapa.definisiHashFunkciju(djbhash);


  const int brojElemenata = 10000;


  std::vector<int> slucajniKljucevi;
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distribucija(1, 100000);

  for (int i = 0; i < brojElemenata; ++i) {
    slucajniKljucevi.push_back(distribucija(gen));
  }

  auto pocetak = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < brojElemenata; ++i) {
    nizMapa[slucajniKljucevi[i]] = "Vrijednost";
  }

  //dodavanje
  auto kraj = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> trajanje = kraj - pocetak;
  std::cout << "NizMapa - Dodavanje: " << trajanje.count() << " sekundi\n";

  pocetak = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < brojElemenata; ++i) {
    binStabloMapa[slucajniKljucevi[i]] = "Vrijednost";
  }
  kraj = std::chrono::high_resolution_clock::now();
  trajanje = kraj - pocetak;
  std::cout << "BinStabloMapa - Dodavanje: " << trajanje.count() << " sekundi\n";

  pocetak = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < brojElemenata; ++i) {
    hashMapa[slucajniKljucevi[i]] = "Vrijednost";
  }
  kraj = std::chrono::high_resolution_clock::now();
  trajanje = kraj - pocetak;
  std::cout << "HashMapa - Dodavanje: " << trajanje.count() << " sekundi\n";

  //pristup
  pocetak = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < brojElemenata; ++i) {
    std::string vrijednost = nizMapa[slucajniKljucevi[i]];
  }
  kraj = std::chrono::high_resolution_clock::now();
  trajanje = kraj - pocetak;
  std::cout << "NizMapa - Pristup: " << trajanje.count() << " sekundi\n";

  pocetak = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < brojElemenata; ++i) {
    std::string vrijednost = binStabloMapa[slucajniKljucevi[i]];
  }
  kraj = std::chrono::high_resolution_clock::now();
  trajanje = kraj - pocetak;
  std::cout << "BinStabloMapa - Pristup: " << trajanje.count() << " sekundi\n";

  pocetak = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < brojElemenata; ++i) {
    std::string vrijednost = hashMapa[slucajniKljucevi[i]];
  }
  kraj = std::chrono::high_resolution_clock::now();
  trajanje = kraj - pocetak;
  std::cout << "HashMapa - Pristup: " << trajanje.count() << " sekundi\n";

//brisanje
  pocetak = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < brojElemenata; ++i) {
    nizMapa.obrisi(slucajniKljucevi[i]);
  }
  kraj = std::chrono::high_resolution_clock::now();
  trajanje = kraj - pocetak;
  std::cout << "NizMapa - Brisanje: " << trajanje.count() << " sekundi\n";

  pocetak = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < brojElemenata; ++i) {
    binStabloMapa.obrisi(slucajniKljucevi[i]);
  }
  kraj = std::chrono::high_resolution_clock::now();
  trajanje = kraj - pocetak;
  std::cout << "BinStabloMapa - Brisanje: " << trajanje.count() << " sekundi\n";

  pocetak = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < brojElemenata; ++i) {
    hashMapa.obrisi(slucajniKljucevi[i]);
  }
  kraj = std::chrono::high_resolution_clock::now();
  trajanje = kraj - pocetak;
  std::cout << "HashMapa - Brisanje: " << trajanje.count() << " sekundi\n";

//Operacije dodavanja, pristupa i brisanja članova su brže u HashMapa implementaciji u usporedbi s NizMapa i BinStabloMapa. Ovo potvrđuje očekivanje jer su hash mape optimizirane za brze operacije u prosječnom slučaju.

//BinStabloMapa ima sporije operacije od NizMapa, što je tipično za binarna stabla pretrage jer imaju složenost logaritamskog vremena.

//HashMapa se automatski prilagođava veličini kako bi održala efikasnost. S druge strane, fiksna veličina NizMapa mora se ručno proširivati, što može rezultirati sporijim operacijama.

//Brisanje članova u HashMapa može biti brže od NizMapa i BinStabloMapa, posebno kada ima puno elemenata. BinStabloMapa ima dodatnu složenost zbog potrebe za održavanjem uređenosti stabla.
  return 0;
}




template <typename spp> class GranaIterator;
template <typename spp> class Grana;
template <typename spp> class Cvor;
template <typename spp> class ListaGraf;

template <typename spp> class UsmjereniGraf {
public:
  UsmjereniGraf() {}
  UsmjereniGraf(int a) {}
  virtual ~UsmjereniGraf() {}

  virtual int dajBrojCvorova() const = 0;
  virtual void postaviBrojCvorova(int a) = 0;
  virtual void dodajGranu(int p, int d, float t) = 0;
  virtual void obrisiGranu(int p, int d) = 0;
  virtual bool postojiGrana(int p, int d) = 0;
  virtual void postaviTezinuGrane(int p, int d, float t) = 0;
  virtual spp dajOznakuGrane(int p, int d) = 0;
  virtual spp dajOznakuCvora(int b) = 0;
  virtual float dajTezinuGrane(int p, int d) = 0;
  virtual void postaviOznakuCvora(int b, spp a) = 0;
  virtual void postaviOznakuGrane(int p, int d, spp a) = 0;
  Grana<spp> dajGranu(int p, int d) { return Grana<spp>(this, p, d); }
  Cvor<spp> dajCvor(int b) { return Cvor<spp>(this, b); }

  GranaIterator<spp> dajGranePocetak() {
    GranaIterator<spp> it(this, 0, -1);
    return ++it;
  }

  GranaIterator<spp> dajGraneKraj() {
    GranaIterator<spp> it(this, -1, -1);
    return it;
  }
  virtual GranaIterator<spp> dajSljedecuGranu(int p, int d) = 0;
};

template <typename spp> class Cvor {

  UsmjereniGraf<spp> *u;
  spp oznaka;
  int broj;
  std::vector<Grana<spp>> grane;
  std::vector<Cvor<spp>> susjedi;

public:
  Cvor(UsmjereniGraf<spp> *a, int b) : u(a), broj(b) {}

  int dajRedniBroj() const { return broj; }

  void postaviOznaku(spp a) { u->postaviOznakuCvora(broj, a); }

  spp dajOznaku() const { return u->dajOznakuCvora(broj); }

  void dodajVezu(Grana<spp> g, Cvor<spp> s) {
    grane.push_back(g);
    susjedi.push_back(s);
  }
  void obrisiVezu(Grana<spp> g, Cvor<spp> s) {

    for (int i = 0; i < susjedi.size(); i++)
      if (s == susjedi[i]) {

        for (int j = i; j < susjedi.size() - 1; j++) {
          susjedi[j] = susjedi[j + 1];
          grane[j] = grane[j + 1];
        }
        break;
      }
    susjedi.pop_back();
    grane.pop_back();
  }

  std::vector<Grana<spp>> dajGrane() const { return grane; }
  std::vector<Cvor<spp>> dajSusjede() const { return susjedi; }
};

template <typename spp> class Grana {

  UsmjereniGraf<spp> *u;
  spp oznaka;
  float tezina;
  int p, d;

public:
  Grana(UsmjereniGraf<spp> *ug, int polazni, int dolazni)
      : u(ug), p(polazni), d(dolazni) {}
  float dajTezinu() const { return u->dajTezinuGrane(p, d); }
  void postaviTezinu(float a) { u->postaviTezinuGrane(p, d, a); }
  void postaviOznaku(spp a) { u->postaviOznakuGrane(p, d, a); }
  spp dajOznaku() const { return u->dajOznakuGrane(p, d); }
  Cvor<spp> dajPolazniCvor() const { return u->dajCvor(p); };
  Cvor<spp> dajDolazniCvor() const { return u->dajCvor(d); };
};

template <typename spp> class GranaIterator {

  int p, d;
  UsmjereniGraf<spp> *u;

public:
  GranaIterator(UsmjereniGraf<spp> *us, int a, int b) : p(a), d(b), u(us){};

  Grana<spp> operator*() { return u->dajGranu(p, d); }
  bool operator==(const GranaIterator &it) const {
    return it.u == u && it.p == p && it.d == d;
  }
  bool operator!=(const GranaIterator &it) const {
    return it.u != u || it.p != p || it.d != d;
  }
  GranaIterator &operator++() {
    GranaIterator<spp> sljedeci = u->dajSljedecuGranu(p, d);
    p = sljedeci.p;
    d = sljedeci.d;
    return *this;
  }
  GranaIterator operator++(int) {
    GranaIterator<spp> ovaj = *this;
    ++(*this);
    return ovaj;
  }
};

template <typename spp> class ListaGraf : public UsmjereniGraf<spp> {

  struct grana {
    spp oznaka;
    float tezina;
    bool postoji;
  };
  std::vector<std::vector<grana>> lista;
  std::vector<spp> oznake;

public:
  ListaGraf(int b) : UsmjereniGraf<spp>(b) { postaviBrojCvorova(b); }
  int dajBrojCvorova() const override { return lista.size(); }

  void postaviBrojCvorova(int a) override {
    grana g;
    g.postoji = false;
    for (int i = 0; i < lista.size(); i++)
      lista[i].resize(a, g);
    std::vector<grana> ubaci(a, g);
    lista.resize(a, ubaci);
    oznake.resize(a);
  }
  bool postojiGrana(int p, int d) override { return lista[p][d].postoji; }

  void dodajGranu(int p, int d, float t) override {
    grana g;
    g.tezina = t;
    g.postoji = true;
    lista[p][d] = g;
  }
  void obrisiGranu(int p, int d) override { lista[p][d].postoji = false; }
  void postaviTezinuGrane(int p, int d, float t) override {
    lista[p][d].tezina = t;
  }
  spp dajOznakuCvora(int b) override { return oznake[b]; }

  spp dajOznakuGrane(int p, int d) override { return lista[p][d].oznaka; }
  float dajTezinuGrane(int p, int d) override { return lista[p][d].tezina; }
  void postaviOznakuCvora(int b, spp a) override { oznake[b] = a; }
  void postaviOznakuGrane(int p, int d, spp a) override {
    lista[p][d].oznaka = a;
  }
  GranaIterator<spp> dajGranePocetak() {
    GranaIterator<spp> it(this, 0, -1);
    ++it;
    return it;
  }
  GranaIterator<spp> dajGraneKraj() {
    GranaIterator<spp> it(this, -1, -1);
    return it;
  }

  GranaIterator<spp> dajSljedecuGranu(int p, int d) override {
    for (int i = p; i < lista.size(); i++) {
      for (int j = 0; j < lista.size(); j++) {
        if (i == p && j <= d)
          continue;

        if (lista[i][j].postoji)
          return GranaIterator<spp>(this, i, j);
      }
    }
    return GranaIterator<spp>(this, -1, -1);
  }
};

template <typename spp>
void bfs(UsmjereniGraf<spp> *g, std::vector<Cvor<spp>> &b, Cvor<spp> c) {
  bool dodati = false;
  for (int i = 0; i < b.size(); i++) {
    if (b[i].dajRedniBroj() == c.dajRedniBroj()) {
      dodati = true;
      break;
    }
  }
  if (!dodati)
    b.push_back(c);
  int br = g->dajBrojCvorova();
  for (int i = 0; i < br; i++)
    if (!dodati)
      bfs(g, b, g->dajCvor(i));
}

template <typename spp>
void dfs(UsmjereniGraf<spp> *g, std::vector<Cvor<spp>> &b, Cvor<spp> c) {
  bool dodati = false;
  for (int i = 0; i < b.size(); i++) {
    if (c.dajRedniBroj() == b[i].dajRedniBroj()) {
      dodati = true;
      break;
    }
  }
  if (!dodati)
    b.push_back(c);
  int brojCvorova = g->dajBrojCvorova();
  for (int i = 0; i < brojCvorova; i++)
    if (g->postojiGrana(c.dajRedniBroj(), i) && !dodati)
      dfs(g, b, g->dajCvor(i));
}
