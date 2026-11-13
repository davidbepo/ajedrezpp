#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <array>

using namespace sf;
using namespace std;

// funciones arrays y utiles globales
int poscasilla[]={0,0,70,140,210,280,350,420,490,560,999};

class Metapieza;

array<array<Metapieza*, 8>, 8> ocupadas{};
array<array<int, 8>, 8> colocupadas{};

int colocaballo[] = {0,2,7};
int colocalfil[] = {0,3,6};
int colocatorre[] = {0,1,8};
int filapieza[] = {0,8,1};

const Texture tpb("peonblanco.png");
const Texture tpn("peonegro.png");
const Texture tcb("caballoblanco.png");
const Texture tcn("caballonegro.png");
const Texture ttb("torreblanca.png");
const Texture ttn("torrenegra.png");
const Texture talb("alfilblanco.png");
const Texture taln("alfilnegro.png");
const Texture trb("reyblanco.png");
const Texture trn("reynegro.png");
const Texture tyb("reinablanca.png");
const Texture tyn("reinanegra.png");

class Metapieza {
    public:
    int casx{};
    int casy{};
    int posx{};
    int posy{};
    int color{};
    int movida{};
    Sprite foto;
    string tipopieza="defecto";
    vector<Vector2i> casposibles;
    Metapieza(int x,int y,int clr, const Texture& tex, string tipo) :
    color{clr}, foto{tex},  tipopieza{tipo} {
        setpos(x,y);
    }
    virtual ~Metapieza() = default;
    void setpos(int x,int y) {
        casx=x;
        casy=y;
        posx=poscasilla[x];
        posy=poscasilla[y];
        foto.setPosition({(float)posx,(float)posy});
        casposibles.clear();
        if(casx<9 and casy<9) {
            colocupadas[y-1][x-1] = this->color;
            ocupadas[y-1][x-1] = this;
        }
    }
    void cambiasilla(int x, int y) {
        ocupadas[casy-1][casx-1] = nullptr;
        colocupadas[casy-1][casx-1] = 0;
        setpos(x,y);
        movida = 1;
    }
    void pintar(RenderWindow &ventana){
        ventana.draw(foto);
    }
    Vector2i casillaocupada() {
        return {casy, casx};
    }
    virtual vector<Vector2i> puedemovera() {
        return casposibles;
    }
    vector<Vector2i> movlineal(int movmax = 8) {
        vector<Vector2i> casposiblesi;
        int casn = 0; //casilla numero
        //arriba, abajo, derecha, izquierda
        bool ar = true, ab = true, de = true, iz = true;
        while (casn < movmax) {
            casn++;
            //la direccion vertical esta invertida :(
            if ((casy-casn > 0 and casy-casn <= 8) and (casx > 0 and casx <= 8) and ar) {//compruba arriba
                ar = colocupadas[casy-casn-1][casx-1] != color;
                if (ar) {
                    casposiblesi.push_back({casx,casy-casn});
                    ar = colocupadas[casy-casn-1][casx-1] != 3-color;
                }
            }
            if ((casy+casn > 0 and casy+casn <= 8) and (casx > 0 and casx <= 8) and ab) {//compruba abajo
                ab = colocupadas[casy+casn-1][casx-1] != color;
                if (ab) {
                    casposiblesi.push_back({casx,casy+casn});
                    ab = colocupadas[casy+casn-1][casx-1] != 3-color;
                }
            }
            if ((casy > 0 and casy <= 8) and (casx-casn > 0 and casx-casn <= 8) and iz) {//compruba izquierda
                iz = colocupadas[casy-1][casx-casn-1] != color;
                if (iz) {
                    casposiblesi.push_back({casx-casn,casy});
                    iz = colocupadas[casy-1][casx-casn-1] != 3-color;
                }
            }
            if ((casy > 0 and casy <= 8) and (casx+casn > 0 and casx+casn <= 8) and de) {//compruba derecha
                de = colocupadas[casy-1][casx+casn-1] != color;
                if (de) {
                    casposiblesi.push_back({casx+casn,casy});
                    de = colocupadas[casy-1][casx+casn-1] != 3-color;
                }
            }
        }
        return casposiblesi;
    }
    vector<Vector2i> movdiagonal(int movmax = 8) {
        vector<Vector2i> casposiblesi;
        int casn = 0; //casilla numero
        //arriba derecha, abajo derecha, arriba izquierda, abajo izquierda
        bool arde = true, abde = true, ariz = true, abiz = true;
        while (casn < movmax) {
            casn++;
            //la direccion vertical esta invertida :(
            if ((casy-casn > 0 and casy-casn <= 8) and (casx+casn+casn > 0 and casx+casn <= 8) and arde) {//compruba arriba derecha
                arde = colocupadas[casy-casn-1][casx+casn-1] != color;
                if (arde) {
                    casposiblesi.push_back({casx+casn,casy-casn});
                    arde = colocupadas[casy-casn-1][casx+casn-1] != 3-color;
                }
            }
            if ((casy+casn > 0 and casy+casn <= 8) and (casx+casn > 0 and casx+casn <= 8) and abde) {//compruba abajo derecha
                abde = colocupadas[casy+casn-1][casx+casn-1] != color;
                if (abde) {
                    casposiblesi.push_back({casx+casn,casy+casn});
                    abde = colocupadas[casy+casn-1][casx+casn-1] != 3-color;
                }
            }
            if ((casy-casn > 0 and casy-casn <= 8) and (casx-casn > 0 and casx-casn <= 8) and ariz) {//compruba arriba izquierda
                ariz = colocupadas[casy-casn-1][casx-casn-1] != color;
                if (ariz) {
                    casposiblesi.push_back({casx-casn,casy-casn});
                    ariz = colocupadas[casy-casn-1][casx-casn-1] != 3-color;
                }
            }
            if ((casy+casn > 0 and casy+casn <= 8) and (casx-casn > 0 and casx-casn <= 8) and abiz) {//compruba abajo izquierda
                abiz = colocupadas[casy+casn-1][casx-casn-1] != color;
                if (abiz) {
                    casposiblesi.push_back({casx-casn,casy+casn});
                    abiz = colocupadas[casy+casn-1][casx-casn-1] != 3-color;
                }
            }
        }
        return casposiblesi;
    }
};

class Metapeon : public Metapieza {
    public:
    Metapeon(int columna, int fila, int color, Texture textura, string tipo) :
    Metapieza(columna, fila, color, textura, tipo) {}
    
    vector<Vector2i> puedemovera() override {
        int lpeon[]={0,-1,1,5,4};
        if ((casy+lpeon[color] > 0 and casy+lpeon[color] <= 8) and (casx > 0 and casx <= 8)) {
            if (colocupadas[casy+lpeon[color]-1][casx-1] == 0) {// comprueba si la siguiente casilla esta libre
                casposibles.push_back({casx,casy+lpeon[color]});
                if (movida == 0 and colocupadas[lpeon[color+2]-1][casx-1] == 0)// comprueba si no se ha movido y  la siguiente casilla esta libre
                    casposibles.push_back({casx,lpeon[color+2]});
            }
        }
        if ((casy+lpeon[color] > 0 and casy+lpeon[color] <= 8) and (casx+1 > 0 and casx+1 <= 8)) {//comprueba si puede comer en diagonal a la derecha
            if (colocupadas[casy+lpeon[color]-1][casx+1-1] == 3-color)
                casposibles.push_back({casx+1,casy+lpeon[color]});
        }
        if ((casy+lpeon[color] > 0 and casy+lpeon[color] <= 8) and (casx-1 > 0 and casx-1 <= 8)) {//comprueba si puede comer en diagonal a la izzquierda
            if (colocupadas[casy+lpeon[color]-1][casx-1-1] == 3-color)
                casposibles.push_back({casx-1,casy+lpeon[color]});
        }
        return casposibles;
    }
};
class Metaballo : public Metapieza {
    public:
    Metaballo(int num, int color, Texture textura, string tipo) :
    Metapieza(colocaballo[num], filapieza[color], color, textura, tipo) {}
    vector<Vector2i> puedemovera() override {
        int listaprobar[]={-2,-1,1,2};
        for (int x : listaprobar) {
            for (int y : listaprobar) {
                if(abs(x) != abs(y)) {
                    if((casy+y > 0 and casy+y<= 8) and (casx+x > 0 and casx+x <= 8)) {
                        if (colocupadas[casy+y-1][casx+x-1] == 0 or 
                        colocupadas[casy+y-1][casx+x-1] == 3-color)
                            casposibles.push_back({casx+x,casy+y});
                }
                }
            }
        }
        return casposibles;
    }
};

class Metalfil : public Metapieza {
    public:
    Metalfil(int num, int color, Texture textura, string tipo) :
    Metapieza(colocalfil[num], filapieza[color], color, textura, tipo) {}
    vector<Vector2i> puedemovera() override {
        casposibles = movdiagonal();
        return casposibles;
    }
};
class Metatorre : public Metapieza {
    public:
    Metatorre(int num, int color, Texture textura, string tipo) :
    Metapieza(colocatorre[num], filapieza[color] ,color, textura, tipo) {}
    vector<Vector2i> puedemovera() override {
        casposibles = movlineal();
        return casposibles;
    }
};
class Metareina : public Metapieza {
    public:
    Metareina(int color, Texture textura, string tipo) :
    Metapieza(4, filapieza[color], color, textura, tipo) {}
    vector<Vector2i> puedemovera() override {
        casposibles = movlineal();
        vector<Vector2i> casposibles2 = movdiagonal();
        casposibles.insert(casposibles.end(), casposibles2.begin(), casposibles2.end());
        return casposibles;
    }
};
class Metarey : public Metapieza {
    public:
    Metarey(int color, Texture textura, string tipo) :
    Metapieza(5, filapieza[color], color, textura, tipo) {}
    vector<Vector2i> puedemovera() override {
        casposibles = movlineal(1);
        vector<Vector2i> casposibles2 = movdiagonal(1);
        casposibles.insert(casposibles.end(), casposibles2.begin(), casposibles2.end());
        return casposibles;
    }
};

class Peonblanco : public Metapeon {
    public:
    Peonblanco(int num) : Metapeon(num, 7, 1, tpb, "Peonblanco") {
        foto.setTexture(tpb);
    }
};
class Peonegro : public Metapeon {
    public:
     Peonegro(int num) : Metapeon(num, 2, 2, tpn, "Peonegro") {
        foto.setTexture(tpn);
    }
};
class Caballoblanco : public Metaballo {
    public:
    Caballoblanco(int num) : Metaballo(num, 1, tcb, "Caballoblanco") {
        foto.setTexture(tcb);
    }
};
class Caballonegro : public Metaballo {
    public:
     Caballonegro(int num) : Metaballo(num, 2, tcn, "Caballonegro"){
        foto.setTexture(tcn);
    }
};
class Torreblanca : public Metatorre {
    public:
    Torreblanca(int num) : Metatorre(num, 1, ttb, "Torreblanca") {
        foto.setTexture(ttb);
    }
};
class Torrenegra : public Metatorre {
    public:
    Torrenegra(int num) : Metatorre(num, 2, ttn, "Torrenegra") {
        foto.setTexture(ttn);
    }
};
class Alfilblanco : public Metalfil {
    public:
    Alfilblanco(int num) : Metalfil(num, 1, talb, "Alfilblanco") {
        foto.setTexture(talb);
    }
};
class Alfilnegro : public Metalfil {
    public:
    Alfilnegro(int num) : Metalfil(num, 2, taln, "Alfilnegro") {
        foto.setTexture(taln);
    }
};
class Reyblanco : public Metarey {
    public:
    Reyblanco() : Metarey(1, trb, "Reyblanco") {
        foto.setTexture(trb);
    }
};
class Reynegro : public Metarey {
    public:
    Reynegro() : Metarey(2, trn, "Reynegro"){
        foto.setTexture(trn);
    }
};
class Reinablanca : public Metareina {
    public:
    Reinablanca() : Metareina(1, tyb, "Reinablanca") {
        foto.setTexture(tyb);
    }
};
class Reinanegra : public Metareina {
    public:
    Reinanegra() : Metareina(2, tyn, "Reinanegra"){
        foto.setTexture(tyn);
    }
};

void comepieza(Metapieza *pieza) {
    pieza->setpos(9,9);
}

Metapieza *sacapieza(int casx, int casy) {
    return ocupadas[casy-1][casx-1];
}

Vector2i sacasilla(Vector2i posraton) {
    int x,y;
    for(int i=1;i<9;i++) {
        if (poscasilla[i] < posraton.x and posraton.x <= poscasilla[i+1])
            x = i;
        if (poscasilla[i] < posraton.y and posraton.y <= poscasilla[i+1])
            y = i;
    }
    return {x,y};
}

int main() {
    RenderWindow ventana(VideoMode({560,560}), "ajedrez");
    const Texture ttablero("tablero-ajedrez.png");
    Sprite tablero(ttablero);
    const Texture tpuntoazul("puntoazul.png");
    Sprite puntoazul(tpuntoazul);
    const Texture tgblancas("gblancas.png");
    Sprite gblancas(tgblancas);
    const Texture tgnegras("gnegras.png");
    Sprite gnegras(tgnegras);
    Peonblanco pb1(1),pb2(2),pb3(3),pb4(4),pb5(5),pb6(6),pb7(7),pb8(8);
    Peonegro pn1(1),pn2(2),pn3(3),pn4(4),pn5(5),pn6(6),pn7(7),pn8(8);
    Caballoblanco cb1(1),cb2(2);
    Caballonegro cn1(1),cn2(2);
    Alfilblanco ab1(1),ab2(2);
    Alfilnegro an1(1),an2(2);
    Torreblanca tb1(1),tb2(2);
    Torrenegra tn1(1),tn2(2);
    Reyblanco reyblanco;
    Reynegro reynegro;
    Reinablanca reinablanca;
    Reinanegra reinanegra;
    /*
    //imprime el tablero(colores)
    for(auto fila : colocupadas) {
        for(auto color : fila) {
                cout << color << " ";
        }
        cout << endl;
    }
    //imprime el tablero(objetos)
    for(auto fila : ocupadas) {
        for(Metapieza* pieza : fila) {
            if(pieza)
                cout << pieza->tipopieza << " ";
            else
                cout << "x ";
        }
        cout << endl;
    }*/

    vector<Vector2i> click,posimov;
    Vector2i posraton,clicasilla,nuevacasilla;
    Metapieza *fichamover = nullptr;
    string turno = "blancas";
    cout << "empiezan las " << turno << endl;
    while (ventana.isOpen()) {
        const auto onClose = [&](const sf::Event::Closed&){
            ventana.close();
        };
        const auto onMousePressed = [&](const sf::Event::MouseButtonPressed& raton){
            click.push_back(raton.position);
        };
        ventana.handleEvents(onClose, onMousePressed);
        
        if (click.size() == 1) {
            posraton = click[0];
            clicasilla = sacasilla(posraton);
            fichamover = sacapieza(clicasilla.x, clicasilla.y);
            if(fichamover) {//si el click es en una casilla con pieza
                if ((fichamover->color == 1 and turno == "negras") or (fichamover->color == 2 and turno == "blancas")) {
                    //limpiar
                    click.clear();
                    fichamover = nullptr;
                } else
                    posimov = fichamover->puedemovera();
            } else {
                click.clear();
                fichamover = nullptr;
            }
        }
        if (click.size() == 2) {
            posraton = click[1];
            nuevacasilla = sacasilla(posraton);
            if(std::find(posimov.begin(), posimov.end(), nuevacasilla) != posimov.end()) {//si el movimiento es valido
                if (ocupadas[nuevacasilla.y-1][nuevacasilla.x-1] != nullptr)
                    comepieza(sacapieza(nuevacasilla.x,nuevacasilla.y));// si la casilla de destino esta ocupada come la pieza
                fichamover->cambiasilla(nuevacasilla.x, nuevacasilla.y);// mueve la pieza
                if(fichamover->color == 1)// cambia el turno
                    turno = "negras";
                if(fichamover->color == 2)
                    turno = "blancas";
            }
            // limpiar siempre
            click.clear();
            fichamover = nullptr;
        }
        ventana.clear();
        ventana.draw(tablero);
        //pinta las piezas
        for(auto fila : ocupadas) {
            for(auto pieza : fila) {
                if(pieza)
                    pieza->pintar(ventana);
            }
        }
        //pinta los puntos azules
        if (click.size() > 0 and posimov.size() > 0) {
            for(auto pos : posimov) {
                puntoazul.setPosition({(float)poscasilla[pos.x], (float)poscasilla[pos.y]});
                ventana.draw(puntoazul);
            }
        }
        if(reynegro.casx > 8)
            ventana.draw(gblancas);
        if(reyblanco.casx > 8)
            ventana.draw(gnegras);
        ventana.display();
        
        sleep(milliseconds(20));//limita a 50 fps
    }
}

