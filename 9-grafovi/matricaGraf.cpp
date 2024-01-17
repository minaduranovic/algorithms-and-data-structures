#include <iostream>
#include <vector>

using namespace std;

template <typename TipOznake> class Cvor;
template <typename TipOznake> class Grana;
template <typename TipOznake> class GranaIterator;

template <typename TipOznake> class UsmjereniGraf {

public:
  UsmjereniGraf(int brCvorova) {}
  virtual ~UsmjereniGraf() = default;
  virtual int dajBrojCvorova() const = 0;
  virtual void postaviBrojCvorova(int brCvorova) = 0;
  virtual void dodajGranu(int polazni, int dolazni, double tezina) = 0;
  virtual void obrisiGranu(int polazni, int dolazni) = 0;
  virtual void postaviTezinuGrane(int polazni, int dolazni, double tezina) = 0;
  virtual double dajTezinuGrane(int polazni, int dolazni) = 0;
  virtual bool postojiGrana(int polazni, int dolazni) = 0;
  virtual void postaviOznakuCvora(int brCvora, TipOznake oznaka) = 0;
  virtual void postaviOznakuGrane(int polazni, int dolazni,
                                  TipOznake oznaka) = 0;
  virtual TipOznake dajOznakuCvora(int brCvora) = 0;
  virtual TipOznake dajOznakuGrane(int polazni, int dolazni) = 0;

  virtual Grana<TipOznake> &dajGranu(int, int) = 0;
  virtual Cvor<TipOznake> &dajCvor(int) = 0;
  virtual GranaIterator<TipOznake> dajGranePocetak() = 0;
  virtual GranaIterator<TipOznake> dajGraneKraj() = 0;
};

template <typename TipOznake> class Grana {
  int pocetak, kraj;
  double tezina;
  TipOznake oznaka;
  UsmjereniGraf<TipOznake> *UGraf;

public:
  Grana(int p, int q, UsmjereniGraf<TipOznake> *g, double tez)
      : pocetak(p), kraj(q), UGraf(g), tezina(tez) {}

  TipOznake dajOznaku() const { return oznaka; }
  double dajTezinu() const { return tezina; }
  Cvor<TipOznake> dajPolazniCvor() { return UGraf->dajCvor(pocetak); }
  Cvor<TipOznake> dajDolazniCvor() { return UGraf->dajCvor(kraj); }

  void postaviOznaku(TipOznake oz) {
    oznaka = oz;
    UGraf->dajGranu(pocetak, kraj).oznaka = oz;
  }
  void postaviTezinu(double t) {
    tezina = t;
    UGraf->dajGranu(pocetak, kraj).tezina = t;
  }
};

template <typename TipOznake> class Cvor {
  int brCvora;
  TipOznake oznaka;
  UsmjereniGraf<TipOznake> *UGraf;

public:
  Cvor(UsmjereniGraf<TipOznake> *ug, int rb) : UGraf(ug), brCvora(rb) {}

  int dajRedniBroj() const { return brCvora; }
  TipOznake dajOznaku() const { return oznaka; }

  void postaviOznaku(TipOznake o) { oznaka = o; }
};

template <typename TipOznake> class GranaIterator {

  int dolazni;
  int polazni;
  int pozicija;
  std::vector<Grana<TipOznake>> grane;
  std::vector<std::vector<bool>> vrijednosti;

  void trazi(int &counter, int &red, int &kolona, bool uslov) {
    for (int i = 0; i < vrijednosti.size(); i++) {
      for (int j = 0; j < vrijednosti[i].size(); j++) {
        if (counter <= pozicija && vrijednosti[i][j]) {
          counter++;
          red = i;
          kolona = j;
        } else if (counter > pozicija) {
          uslov = true;
          break;
        }
      }
      if (uslov == true)
        break;
    }
  }

public:
  GranaIterator(std::vector<std::vector<bool>> v,
                std::vector<Grana<TipOznake>> g, int p, int k, int poz)
      : dolazni(k), polazni(p), pozicija(poz), grane(g), vrijednosti(v) {}

  Grana<TipOznake> operator*() {
    int counter = 0;
    int red = 0;
    int kolona = 0;

    trazi(counter, red, kolona, false);

    for (int i = 0; i < grane.size(); i++) {
      if (grane[i].dajPolazniCvor().dajRedniBroj() == red &&
          grane[i].dajDolazniCvor().dajRedniBroj() == kolona)
        return grane[i];
    }

    throw std::logic_error("Trazena grana ne postoji");
  }

  bool operator==(const GranaIterator &iter) const {
    int counter = 0;
    int red = 0;
    int kolona = 0;

    trazi(counter, red, kolona, false);

    if (vrijednosti[red][kolona] == iter.vrijednosti[red][kolona])
      return true;
    return false;
  }
  bool operator!=(GranaIterator iter) {
    int counter = 0;
    int red = 0;
    int kolona = 0;

    trazi(counter, red, kolona, false);

    if (vrijednosti[red][kolona] != iter.vrijednosti[red][kolona])
      return false;
    if (pozicija == grane.size())
      return false;
    return true;
  }
  bool operator!=(const GranaIterator &it) const {
    int counter = 0;
    int red = 0;
    int kolona = 0;

    trazi(counter, red, kolona, false);

    if (vrijednosti[red][kolona] != it.vrijednosti[red][kolona])
      return false;
    if (pozicija == grane.size())
      return false;
    return true;
  }
  GranaIterator &operator++() {
    pozicija++;
    return *this;
  }
  GranaIterator operator++(int) {
    GranaIterator temp = *this;
    ++*this;
    return temp;
  }
};

template <typename TipOznake>
class MatricaGraf : public UsmjereniGraf<TipOznake> {
  std::vector<Grana<TipOznake>> grane;
  std::vector<Cvor<TipOznake>> cvorovi;
  std::vector<std::vector<bool>> vrijednosti;

public:
  MatricaGraf(int brCvorova) : UsmjereniGraf<TipOznake>(brCvorova) {
    if (brCvorova < 0)
      throw std::logic_error("Broj cvorova ne smije biti negativan!");

    vrijednosti.resize(brCvorova);

    for (int i = 0; i < brCvorova; i++) {
      vrijednosti[i].resize(brCvorova);
      cvorovi.push_back(Cvor<TipOznake>(this, i));
      for (int j = 0; j < brCvorova; j++)
        vrijednosti[i][j] = false;
    }
  }

  ~MatricaGraf() {}

  MatricaGraf(const MatricaGraf<TipOznake> &other) {
    grane = other.grane;
    cvorovi = other.cvorovi;
    vrijednosti = other.vrijednosti;
  }

  MatricaGraf<TipOznake> &operator=(const MatricaGraf<TipOznake> &other) {
    if (&other != this) {
      grane = other.grane;
      cvorovi = other.cvorovi;
      vrijednosti = other.vrijednosti;
    }
    return *this;
  }

  void postaviBrojCvorova(int brCvorova) override {
    if (brCvorova < cvorovi.size())
      throw std::logic_error("Broj cvorova manji od prethodnog!");

    if (vrijednosti.size() == brCvorova)
      return;

    for (int i = vrijednosti.size(); i < brCvorova; i++) {
      cvorovi.push_back(Cvor<TipOznake>(this, i));
    }

    int prethodna = vrijednosti.size();

    vrijednosti.resize(brCvorova);

    int ubaci = 0;
    for (int i = 0; i < brCvorova; i++) {
      vrijednosti[i].resize(brCvorova);

      if (i >= prethodna)
        ubaci = 0;
      else
        ubaci = prethodna;

      for (int j = ubaci; j < brCvorova; j++) {
        vrijednosti[i][j] = false;
      }
    }
  }
  void postaviTezinuGrane(int p, int k, double t) override {
    for (int i = 0; i < grane.size(); i++) {
      if (p == grane[i].dajPolazniCvor().dajRedniBroj() &&
          k == grane[i].dajDolazniCvor().dajRedniBroj()) {
        grane[i].postaviTezinu(t);
        return;
      }
    }
    throw std::logic_error("Ne postoji grana!");
  }

  void postaviOznakuCvora(int brCvora, TipOznake o) override {
    cvorovi[brCvora].postaviOznaku(o);
  }
  void postaviOznakuGrane(int p, int k, TipOznake o) override {

    for (int i = 0; i < grane.size(); i++) {
      if (p == grane[i].dajPolazniCvor().dajRedniBroj() &&
          k == grane[i].dajDolazniCvor().dajRedniBroj()) {
        grane[i].postaviOznaku(o);
        return;
      }
    }
    throw std::domain_error("Ne postoji grana!");
  }

  void dodajGranu(int p, int k, double t = 0) override {
    if (!postojiGrana(p, k)) {
      vrijednosti[p][k] = true;
      grane.push_back(Grana<TipOznake>(p, k, this, t));
    } else
      throw std::logic_error("Grana vec postoji!");
  }
  void obrisiGranu(int p, int k) override {
    for (int i = 0; i < grane.size(); i++) {
      if (grane[i].dajPolazniCvor().dajRedniBroj() == p &&
          grane[i].dajDolazniCvor().dajRedniBroj() == k) {
        vrijednosti[p][k] = false;
        grane.erase(grane.begin() + i);
        return;
      }
    }

    throw std::logic_error("Granu koju zelite obrisati ne postoji!");
  }

  int dajBrojCvorova() const override { return cvorovi.size(); }
  double dajTezinuGrane(int p, int k) override {
    for (int i = 0; i < grane.size(); i++) {
      if (p == grane[i].dajPolazniCvor().dajRedniBroj() &&
          k == grane[i].dajDolazniCvor().dajRedniBroj()) {
        return grane[i].dajTezinu();
      }
    }
    throw std::logic_error("Trazena grana ne postoji!");
  }
  TipOznake dajOznakuCvora(int brCvora) override {
    return cvorovi[brCvora].dajOznaku();
    throw std::logic_error("Trazeni cvor ne postoji!");
  }
  TipOznake dajOznakuGrane(int p, int k) override {
    for (int i = 0; i < grane.size(); i++) {
      if (p == grane[i].dajPolazniCvor().dajRedniBroj() &&
          k == grane[i].dajDolazniCvor().dajRedniBroj()) {
        return grane[i].dajOznaku();
      }
    }

    throw std::logic_error("Trazena grana ne postoji!");
  }
  Grana<TipOznake> &dajGranu(int p, int k) override {
    for (int i = 0; i < grane.size(); i++) {
      if (grane[i].dajPolazniCvor().dajRedniBroj() == p &&
          grane[i].dajDolazniCvor().dajRedniBroj() == k)
        return grane[i];
    }
    throw std::logic_error("Trazena grana ne postoji!");
  }
  Cvor<TipOznake> &dajCvor(int i) override { return cvorovi[i]; }
  GranaIterator<TipOznake> dajGranePocetak() override {
    return GranaIterator<TipOznake>(
        vrijednosti, grane, grane[0].dajPolazniCvor().dajRedniBroj(),
        grane[0].dajDolazniCvor().dajRedniBroj(), 0);
  }
  GranaIterator<TipOznake> dajGraneKraj() override {
    return GranaIterator<TipOznake>(
        vrijednosti, grane,
        grane[grane.size() - 1].dajPolazniCvor().dajRedniBroj(),
        grane[grane.size() - 1].dajDolazniCvor().dajRedniBroj(), 0);
  }

  bool postojiGrana(int p, int k) override { return vrijednosti[p][k]; }
};


int main() { return 0; }