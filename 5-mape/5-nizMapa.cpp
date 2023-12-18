#include <iostream>

using namespace std;

template <typename tipKljuca, typename tipVrijednosti> class Mapa {
public:
  virtual ~Mapa() {}
  virtual tipVrijednosti &operator[](const tipKljuca &kljuc) = 0;
  virtual const tipVrijednosti &operator[](const tipKljuca &kljuc) const = 0;
  virtual int brojElemenata() const = 0;
  virtual void obrisi() = 0;
  virtual void obrisi(const tipKljuca &kljuc) = 0;
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

  const tipVrijednosti &operator[](const tipKljuca &kljuc) const override {
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

  NizMapa<int, std::string> mapa;

  mapa[1] = "Jedan";
  mapa[2] = "Dva";
  mapa[3] = "Tri";

  for (int i = 1; i <= mapa.brojElemenata(); i++) {
    std::cout << "mapa[" << i << "] = " << mapa[i] << std::endl;
  }
  std::cout << std::endl;
  mapa.obrisi(2);

  for (int i = 1; i <= mapa.brojElemenata(); i++) {
    std::cout << "mapa[" << i << "] = " << mapa[i] << std::endl;
  }

  mapa.obrisi();

  std::cout << "Broj elemenata nakon brisanja: " << mapa.brojElemenata();

  return 0;
}