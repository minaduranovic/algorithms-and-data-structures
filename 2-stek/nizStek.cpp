#include <iostream>

using namespace std;

template <typename tip> class Stek {
private:
  tip **lniz;
  int kapacitet;
  int Vrh;

  void realociraj() {
    int noviKap = kapacitet * 2;
    tip **noviLniz = new tip *[noviKap];

    for (int i = 0; i <Vrh+1; i++) {
      noviLniz[i] = lniz[i];
    }

    delete[] lniz;
    lniz = noviLniz;
    kapacitet = noviKap;
  }

public:
  Stek() {
    kapacitet = 10;
    Vrh = -1;
    lniz = new tip *[kapacitet];
    for (int i = 0; i < kapacitet; i++) {
      lniz[i] = nullptr;
    }
  }

  ~Stek() {
    for (int i = 0; i < Vrh+1; i++) {
      delete lniz[i];
    }
    delete[] lniz;
  }
  int brojElemenata() const  { return Vrh+1; }

  Stek(const Stek &n) {
    kapacitet = n.kapacitet;
    Vrh= n.Vrh;

    lniz = new tip *[kapacitet];
    for (int i = 0; i < Vrh+1; i++) {
      lniz[i] = new tip(*(n.lniz[i]));
    }
  }
  Stek &operator=(const Stek &n) {
    if (this == &n) {
      return *this;
    }
    for (int i = 0; i < Vrh+1; i++) {
      delete lniz[i];
    }
    delete[] lniz;

    kapacitet = n.kapacitet;
    Vrh = n.Vrh;

    lniz = new tip *[kapacitet];
    for (int i = 0; i < Vrh+1; i++) {
      lniz[i] = new tip(*(n.lniz[i]));
    }

    return *this;
  }

  tip skini()  {
    if (Vrh == -1 ) {
      throw std::runtime_error("Prazan stek");
    }
    tip el=*(lniz[Vrh]);
    delete lniz[Vrh];
    lniz[Vrh] = nullptr;
    Vrh--;

    return el;
  }

 tip& vrh() {
    if (Vrh == -1 ) {
      throw std::runtime_error("Prazan stek");
    }

    return *(lniz[Vrh]);
  }

  void brisi()  {
    if (Vrh == -1)
      throw std::runtime_error("Prazan stek.");

     for (int i = 0; i < Vrh+1; i++) {
      delete lniz[i];
    }
    Vrh=-1;
  }


  void stavi(const tip &el)  {
    if (Vrh+1 >= kapacitet) {
      realociraj();
    }
     lniz[++Vrh]=new tip(el);
    }
 
};

int main(){
  Stek<int> stek;
  cout << "Broj elemenata: " << stek.brojElemenata() << endl;
  stek.stavi(1);
  stek.stavi(2);
  stek.stavi(3);
  cout << "Broj elemenata: " << stek.brojElemenata() << endl;
  cout << "Vrh: " << stek.vrh() << endl;
  int skini_elem = stek.skini();
  cout << "Broj elemenata: " << stek.brojElemenata() << endl;
  Stek<int> kopirani_stek(stek);
  cout << "Broj elemenata: " << kopirani_stek.brojElemenata() << endl;
  kopirani_stek.brisi();
  cout << "Broj elemenata: " << kopirani_stek.brojElemenata() << endl;

    return 0;
}