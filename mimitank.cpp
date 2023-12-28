///ATAMNA MIRYAM (droit d'auteur)

#include <iostream>
#include <Grapic.h>
#include <cmath>
#include <time.h>
using namespace grapic;
using namespace std;

const int DIMW=500;
const int MAX_PART=4;
const int MAXPROJ=50;
const int MAX=100;
const float dt=0.1f;
const int MAXCIBLES=10;
const int MAXCOLIS=10;


float to_rad (float a_deg)
{
    return a_deg*M_PI/180.0f; //f=nombre réel
}

struct vec2
{
    float x,y;
};

vec2 make_vec2 (float x, float y)
{
    struct vec2 r;
    r.x=x;
    r.y=y;
    return r;
}

vec2 operator+ (vec2 a, vec2 b)
{
    vec2 c;
    c.x=a.x+b.x;
    c.y=a.y+b.y;
    return c;
}


vec2 operator- (vec2 a, vec2 b)
{
    vec2 c;
    c.x=a.x-b.x;
    c.y=a.y-b.y;
    return c;
}

vec2 operator* (vec2 a, vec2 b)
{
    vec2 c;
    c.x=a.x*b.x-a.y*b.y;
    c.y=a.x*b.y+a.y*b.x;
    return c;
}

vec2 operator* (float lambda, vec2 b)
{
    struct vec2 c;
    c.x=b.x*lambda;
    c.y=b.y*lambda;
    return c;
}

vec2 operator/ (vec2 b, float a)
{
    return make_vec2(b.x/a,b.y/a);
}

vec2 make_vec2_exp(float r, float theta_deg)
{
    struct vec2 c;
    c.x=r*cos(to_rad(theta_deg));
    c.y=r*sin(to_rad(theta_deg));
    return c;
}

vec2 rotation (vec2 p, float cx, float cy, float theta_deg)
{
    vec2 c;
    c=make_vec2(cx,cy);
    p=((p-c)*make_vec2_exp(1,theta_deg)+c);
    return p;
}

float norm(const vec2 v)
{
    return sqrt(v.x*v.x + v.y*v.y);
}

float distance(const vec2& a, const vec2& b)
{
    return norm(a-b);
}

struct proj ///struct projectile
{
    vec2 p; //position
    float m; //masse
    vec2 v; //vitesse
    vec2 f; //force
    int id; //identifiant du tank
    bool actif; //pour savoir si il est mort ou pas
};

struct tank
{
    vec2 p; //position
    vec2 v;  //vitesse
    vec2 canon; //pour tirer
    int angle; //pour faire tourner le tank
    vec2 direction; //pour donner la direction du vecteur
    bool vie; //pour savoir si il est en vie
    proj pr[MAXPROJ]; //tableau de projectiles
    int nb_proj;
    bool peut_tirer; //pour savoir si on peut tirer
    float temps; //pr tirer proj a temps d'écart
    Image imtank;
    int vies;
    int touch_c; //nb de cibles touchées
    float puissance; //ce qu'on va ajt au temps quand on recup les caisses de munitions pr tirer plus vite
    bool bouclier; //vrai lorsqu'on a un bouclier faux sinn
};

struct cible
{
    vec2 pos; ///position
    int duree_vie;
    bool test;
};

struct cibles
{
    cible cib[MAXCIBLES];
};


struct colis
{
    vec2 pos;
    int duree_vie;
    int temps;
};

struct world
{
    tank tk[2];
    int nb_joueurs; //nombre de joueurs
    Menu m;
    bool etat; //pour savoir si on est dans le menu ou dans le jeu
    colis et[MAXCOLIS];
    int nb_colis;
    Image fond, coeur, cible,bouton1,bouton2,Colis,regles,j1gagne,j2gagne,bouclier,etoile1,etoile2,etoile3;
    cibles c; //tableau de cible (je l'ai fais comme ça pcq ça beugait sinn
    int nb_cibles;
    float temps;
    float temps_final;
};

void init_tank(tank &pa,float x,float y)
{
    pa.p=make_vec2(x,y);
    pa.v=make_vec2(0,0);
    pa.direction=make_vec2(x+1,y);
    pa.canon=make_vec2(x+20,y);
    pa.angle = 0;
    pa.vie=true;
    pa.nb_proj = 0;
    pa.peut_tirer=true;
    pa.temps=0;
    pa.puissance=1;
    pa.vies=3;
    pa.touch_c=0;
    pa.bouclier=false;
    for(int i=0; i<MAXPROJ; i++)
    {
        pa.pr[i].actif=false;
    }
}

void init_cibles(world &w)
{
    w.nb_cibles=5;
    for(int i=0;i<w.nb_cibles;i++)
    {
        w.c.cib[i].duree_vie=-1;
    }
    w.c.cib[0].pos=make_vec2(450,450); //y (mouvement par rapport à l'axe y)
    w.c.cib[1].pos=make_vec2(450,270); //y
    w.c.cib[2].pos=make_vec2(450,140); //y
    w.c.cib[3].pos=make_vec2(350,50);   //x
    w.c.cib[4].pos=make_vec2(350,450);  //x
}

void init_world(world &w)
{

    w.nb_joueurs=0;
    init_tank(w.tk[0],50,DIMW-50);
    w.tk[0].imtank=image("../data/miniprojet/tankbleu.png",true);

    init_tank(w.tk[1],DIMW-50,50);
    w.tk[1].imtank=image("../data/miniprojet/tankrouge.png",true);

    w.fond=image("../data/miniprojet/mapMIMI.png",true);
    w.coeur=image("../data/miniprojet/coeur.png",true);
    w.cible=image("../data/miniprojet/cibleMIMI.png",true);
    w.bouton1=image("../data/miniprojet/bouton1.png",true);
    w.bouton2=image("../data/miniprojet/bouton2.png",true);
    w.Colis=image("../data/miniprojet/boiteDeMune.png",true);
    w.regles=image("../data/miniprojet/regles.png",true);
    w.j1gagne=image("../data/miniprojet/j1gg.png",true);
    w.j2gagne=image("../data/miniprojet/j2gg.png",true);
    w.bouclier=image("../data/miniprojet/bouclierMimi.png",true);
    w.etoile1=image("../data/miniprojet/3et.png",true);
    w.etoile2=image("../data/miniprojet/2et.png",true);
    w.etoile3=image("../data/miniprojet/1et.png",true);


    ///on init les colis

    for(int i=0;i<MAXCOLIS;i++)
    {
        w.et[i].pos.x=irand(50,450);
        w.et[i].pos.y=irand(50,450);
        w.et[i].duree_vie=0;
    }
    w.et[0].temps=10;
    w.nb_colis=0;
    w.temps=0;
    w.temps_final=0;

    ///on init le menu

    w.etat=false;
    menu_add(w.m,"JOUEURS");
    menu_add(w.m,"JEU1");
    menu_add(w.m,"JEU2");
    menu_add(w.m,"CLASSEMENT");
    menu_setSelect(w.m,0);
}

void draw_colis (world w)
{
    for(int i=0;i<MAXCOLIS;i++)
    {
        if(w.et[i].duree_vie==-1)
        {
                color(253,248,127);
                if(w.nb_colis%2==0) //le colis va alterner entre un bouclier une fois une caisse de munitions
                {
                    image_draw(w.Colis,w.et[i].pos.x-25,w.et[i].pos.y-25,40,40);
                }
                else{
                    image_draw(w.bouclier,w.et[i].pos.x-25,w.et[i].pos.y-25,40,40);
                }
        }

    }
}

void draw_cibles(world w)
{
    for(int i=0;i<w.nb_cibles;i++)
    {
        if(w.c.cib[i].duree_vie==-1)
        {
            image_draw(w.cible,w.c.cib[i].pos.x-25,w.c.cib[i].pos.y-25,50,50);
        }
    }
}

void draw_tank(tank pa)
{
    color(209,104,193);
    line(pa.p.x,pa.p.y,pa.canon.x,pa.canon.y);
    image_draw(pa.imtank,pa.p.x-20,pa.p.y-20,40,40,-pa.angle);
}

void draw_projectile(tank t)
{
    int i;
    color(0,0,0);
    for (i=0; i<MAXPROJ; i++)
        if (t.pr[i].actif)
            circleFill(t.pr[i].p.x,t.pr[i].p.y,5);
}

void draw_world (world w)
{
    for(int i=0; i<w.nb_joueurs; i++)
    {
        if(w.tk[i].vies>0)
        {
            if(!w.tk[i].bouclier)
            {
                draw_tank(w.tk[i]);
            }

            else{
                draw_tank(w.tk[i]);
                image_draw(w.bouclier,w.tk[i].p.x-20,w.tk[i].p.y-20,40,40); //si le tank a le bouclier on le dessine au dessus
            }
            draw_projectile(w.tk[i]);
        }
        for (int j=0;j<w.tk[i].vies;j++)
        {
            if(i==0)
            {
                image_draw(w.coeur,430+15*j,440,15,15); //on dessine des coeurs pour les vies
            }
            else{
                image_draw(w.coeur,30+15*j,40,15,15);
            }
        }
    }

}

vec2 direction(tank t)
{
    float angle2 = to_rad(t.angle);
    vec2 direc = make_vec2(cos(angle2),sin(angle2));
    return direc;
}

void tirer_proj(tank &t)
{
    int ida=-1;
    for(int i=0; i<MAXPROJ; i++)
    {
        if(t.pr[i].actif==false)
        {
            ida=i;
        }
    }
    if (ida<0) return;

    t.pr[ida].p.x=t.p.x; //on initialise la position du projectile  à la position du tank qui le tire
    t.pr[ida].p.y=t.p.y;
    t.pr[ida].v = direction(t);
    t.pr[ida].actif=true;
    t.nb_proj++;

}


bool estdansleTank (tank t, proj p) ///collisions avec le tank et un proj
{

    if(distance(p.p,t.p)<20)
    {
        return true;
    }
    else
        return false;

}

void update_colis(world &w)
{
        for(int j=0;j<w.nb_joueurs;j++)
        {
            if(distance(w.tk[j].p,w.et[w.nb_colis].pos)<20)
            {
               w.et[w.nb_colis].duree_vie=0; //pour qu'il ne soit plus affiché
               w.et[w.nb_colis].temps=elapsedTime();
               w.et[w.nb_colis].pos=make_vec2(-10,-10);
               if((w.tk[j].puissance>0.3)&&(w.nb_colis%2==0))
               {
                    w.tk[j].puissance=w.tk[j].puissance-0.2; //pour pouvoir tirer plus vite
               }else{
                    w.tk[j].bouclier=true; //pour que les collisions n'enlevent plus de vie
              }
               w.nb_colis++;
            }
        }
    if(elapsedTime()>w.et[w.nb_colis-1].temps+7) //toutes les 7 secondes un colis apparait
    {
        w.et[w.nb_colis].duree_vie=-1; //pour que le procgain s'affiche
        w.tk[0].bouclier=false; //pour que le bouclier s'arrete
        w.tk[1].bouclier=false;
    }

}

void update_cibles(world & w)
{

    vec2 deplacemt=make_vec2(1,1); //le vecteur de deplacement pour les cibles

    for(int i=0;i<w.nb_cibles;i++)
    {
        for(int j=0;j<MAXPROJ;j++)
        if(distance(w.c.cib[i].pos,w.tk[0].pr[j].p)<20)
        {
            w.c.cib[i].duree_vie=0; //pour qu'il ne soit plus affiché
            w.c.cib[i].pos=make_vec2(-50,-50); //on bouge la cible (oui c'est de la triche un peu)
            w.tk[0].pr[j].p = make_vec2(-10,-10) ; //et on bouge le projectile
            w.tk[0].touch_c++; //+1 au nb de cibles touchées
        }
    }

    if(w.c.cib[0].pos.y>450) //pr qu'il fasse des allers retour
        w.c.cib[0].test=true;
    if(w.c.cib[0].pos.y<350)
        w.c.cib[0].test=false;

            if(!w.c.cib[0].test)
                w.c.cib[0].pos.y=w.c.cib[0].pos.y+1.8*deplacemt.y;
            else
                w.c.cib[0].pos.y=w.c.cib[0].pos.y-1.8*deplacemt.y;

    if(w.c.cib[1].pos.y>270)
        w.c.cib[1].test=true;
    if(w.c.cib[1].pos.y<170)
        w.c.cib[1].test=false;

            if(!w.c.cib[1].test)
                w.c.cib[1].pos.y=w.c.cib[1].pos.y+1.8*deplacemt.y;
            else
                w.c.cib[1].pos.y=w.c.cib[1].pos.y-1.8*deplacemt.y;

    if(w.c.cib[2].pos.y>140)
        w.c.cib[2].test=true;
    if(w.c.cib[2].pos.y<40)
        w.c.cib[2].test=false;

            if(!w.c.cib[2].test)
                w.c.cib[2].pos.y=w.c.cib[2].pos.y+3*deplacemt.y;
            else
                w.c.cib[2].pos.y=w.c.cib[2].pos.y-3*deplacemt.y;


    if(w.c.cib[3].pos.x>350)
        w.c.cib[3].test=true;
    if(w.c.cib[3].pos.x<100)
        w.c.cib[3].test=false;

            if(!w.c.cib[3].test)
                w.c.cib[3].pos.x=w.c.cib[3].pos.x+2*deplacemt.x;
            else
                w.c.cib[3].pos.x=w.c.cib[3].pos.x-2*deplacemt.x;

    if(w.c.cib[4].pos.x>350)
        w.c.cib[4].test=true;
    if(w.c.cib[4].pos.x<100)
        w.c.cib[4].test=false;

            if(!w.c.cib[4].test)
                w.c.cib[4].pos.x=w.c.cib[4].pos.x+1.5*deplacemt.x;
            else
                w.c.cib[4].pos.x=w.c.cib[4].pos.x-1.5*deplacemt.x;


}

void update_tank(tank &pa)
{
    pa.p=pa.p+dt*pa.v;
}


void update_proj1(tank &t) ///on update les projectiles du tank1
{
    if ((isKeyPressed(SDLK_RSHIFT))&&(t.peut_tirer))
    {
        tirer_proj(t);
        t.peut_tirer=false; //on tire a intervalle de temps régulier
        t.temps=elapsedTime();

    }

   if(elapsedTime()>t.temps+t.puissance) //selon la puissance on peut tirer plus vite (qui change quand on prend des caisses de mun)
    {
        t.peut_tirer=true;
    }

    for(int i=0; i<MAXPROJ; i++)
    {
        if((t.pr[i].actif==true)&&(t.pr[i].p.x!=-10)&&(t.pr[i].p.y!=-10)) //si le proj est a (-10,10) où on l'a tej talheur on ne l'update plus
        {
            t.pr[i].p = t.pr[i].p + 4*t.pr[i].v;
        }
    }

}

void update_proj2(tank &t) ///même chose avec les projectiles du tank2
{
    if ((isKeyPressed(SDLK_LSHIFT))&&(t.peut_tirer))
    {
        tirer_proj(t);
        t.peut_tirer=false;
        t.temps=elapsedTime();
    }

    if(elapsedTime()>t.temps+t.puissance)
    {
        t.peut_tirer=true;
    }

    for(int i=0; i<MAXPROJ; i++)
    {
        if((t.pr[i].actif==true)&&(t.pr[i].p.x!=-10)&&(t.pr[i].p.y!=-10))
        {
            t.pr[i].p = t.pr[i].p + 4*t.pr[i].v;
        }

    }
}

void update_tank_nouv1(world &w) ///update du tank1
{
    vec2 zero=make_vec2(0,0);
    vec2 a=w.tk[0].direction;
    vec2 oldpos;
    vec2 oldcanon; //on garde la position du canon pour les collisions avec les murs
    oldpos=w.tk[0].p; //on garde la position du tank


    if(isKeyPressed(SDLK_LEFT)) //pour pivoter à gauche
    {
        w.tk[0].direction=rotation(w.tk[0].direction,w.tk[0].p.x,w.tk[0].p.y,2);
        w.tk[0].canon=rotation(w.tk[0].canon,w.tk[0].p.x,w.tk[0].p.y,2);
        w.tk[0].angle +=2;
    }
    if(isKeyPressed(SDLK_RIGHT)) //à droite
    {
        w.tk[0].direction=rotation(w.tk[0].direction,w.tk[0].p.x,w.tk[0].p.y,-2);
        w.tk[0].canon=rotation(w.tk[0].canon,w.tk[0].p.x,w.tk[0].p.y,-2);
        w.tk[0].angle-=2;
    }
    oldcanon=w.tk[0].canon;
    if(isKeyPressed(SDLK_UP)) //pour avancer
    {
        vec2 direc = direction(w.tk[0]);
        w.tk[0].p = w.tk[0].p + direc;
        w.tk[0].canon = w.tk[0].canon + direc;
    }
    if((w.tk[0].p.x<35)||(w.tk[0].p.x>DIMW-35)||(w.tk[0].p.y<45)||(w.tk[0].p.y>DIMW-45)) //collisions avec les murs
    {
        w.tk[0].p=oldpos;
        w.tk[0].canon=oldcanon;
    }

    for(int j=0;j<MAXPROJ;j++)
    {
        if ((estdansleTank(w.tk[0],w.tk[1].pr[j]))&&(!w.tk[0].bouclier)) //collisions avec les proj du tank2
        {
            w.tk[0].vies-- ;
            w.tk[1].pr[j].p = make_vec2(-10,-10);  //on tej le projectiles là où on ne l'update plus
        }
    }

    update_tank(w.tk[0]);
}

void update_tank_nouv2(world &w) ///meme chose avec le tank2 j'ai séparé pour pas confondre les touches et tout mais c pareil
{
    vec2 zero=make_vec2(0,0);
    vec2 a=w.tk[1].direction;
    vec2 oldpos;
    vec2 oldcanon;
    oldpos=w.tk[1].p; ///position du tank


    if(isKeyPressed(SDLK_s))
    {
        w.tk[1].direction=rotation(w.tk[1].direction,w.tk[1].p.x,w.tk[1].p.y,2);
        w.tk[1].canon=rotation(w.tk[1].canon,w.tk[1].p.x,w.tk[1].p.y,2);
        w.tk[1].angle +=2;
    }
    if(isKeyPressed(SDLK_f))
    {
        w.tk[1].direction=rotation(w.tk[1].direction,w.tk[1].p.x,w.tk[1].p.y,-2);
        w.tk[1].canon=rotation(w.tk[1].canon,w.tk[1].p.x,w.tk[1].p.y,-2);
        w.tk[1].angle-=2;
    }
    oldcanon=w.tk[1].canon;
    if(isKeyPressed(SDLK_e))
    {
        vec2 direc = direction(w.tk[1]);
        w.tk[1].p = w.tk[1].p + direc;
        w.tk[1].canon = w.tk[1].canon + direc;
    }
    if((w.tk[1].p.x<0)||(w.tk[1].p.x>DIMW)||(w.tk[1].p.y<0)||(w.tk[1].p.y>DIMW))
    {
        w.tk[1].p=oldpos;
        w.tk[1].canon=oldcanon;
    }

    for(int j=0;j<MAXPROJ;j++)
    {
        if ((estdansleTank(w.tk[1],w.tk[0].pr[j]))&&(!w.tk[1].bouclier))
        {
            w.tk[1].vies-- ;
            w.tk[0].pr[j].p = make_vec2(-10,-10) ;
        }
    }


    update_tank(w.tk[1]);
}


bool start(int x, int X, int y, int Y) ///pour savoir si on appuie dans une zone
{
    int a,b;
    mousePos(a,b);
    if((isMousePressed(SDL_BUTTON_LEFT))&&(a>x)&&(a<X)&&(b>y)&&(b<Y))
        return true;
    else
        return false;
}

void jeu1 (world &w) ///mode de jeu à 1 joueur
{
    float t;
    t=elapsedTime()-w.temps; //on garde le temps
    color(0,0,0);
    print(40,45,t);
    draw_world(w);
    draw_cibles(w);
    update_tank_nouv1(w);
    update_proj1(w.tk[0]);
    update_cibles(w);
    w.temps_final=t;
}

void jeu2 (world &w) ///mode de jeu à 2 joueurs
{
    draw_world(w);
    draw_colis(w);
    update_colis(w);
    update_tank_nouv1(w);
    update_tank_nouv2(w);
    update_proj1(w.tk[0]);
    update_proj2(w.tk[1]);
}

void start_game(world &w)
{
    backgroundColor(255,255,255);

    ///j'y laisse au cas ou y'a un pb
    /*int x,y;
    mousePos(x,y);
    print(450,450,x);
    print(450,430,y);*/

    image_draw(w.bouton1,60,150,175,175);
    image_draw(w.bouton2,285,155,175,175);
    image_draw(w.regles,110,240,300,300);

    if(start(80,220,185,295)) //quand on appuie sur le bouton 1
    {
        w.nb_joueurs=1;
        w.temps=elapsedTime();
        menu_setSelect(w.m,1);
    }
    if(start(300,440,185,295)) //quand on appuie sur le bouton 2
    {
        w.nb_joueurs=2;
        menu_setSelect(w.m,2);
    }
}


int main (int, char**)
{
    world w;
    bool  stop=false;
    winInit("mini-projet",DIMW,DIMW);
    setKeyRepeatMode(true);
    srand(time(NULL));
    init_world(w);
    init_cibles(w);
    do
    {
        winClear();
        image_draw(w.fond,0,0,DIMW,DIMW);
        if(menu_select(w.m)==0)
        {
            start_game(w);
        }
        else if(menu_select(w.m)==1)
        {
            jeu1(w);
            if(w.tk[0].touch_c==5) //si on a touché les 5 cibles
            {
                menu_setSelect(w.m,3);
            }
        }
         else if(menu_select(w.m)==2)
        {
            jeu2(w);
            if((w.tk[0].vies==0)||(w.tk[1].vies==0)) //si un des 2 meurs
            {
                menu_setSelect(w.m,3);
            }
        }
        if(menu_select(w.m)==3)
        {
            if(w.nb_joueurs==2)
            {
                if(w.tk[0].vies==0)
                {
                    image_draw(w.j2gagne,0,0,500,500);
                }
                if(w.tk[1].vies==0)
                {
                   image_draw(w.j1gagne,0,0,500,500);
                }
            }
            else{
                    color(0,0,0);
                    if(w.temps_final<15) //affiche 1 ou plusieurs etoiles selon le temps
                    {
                        image_draw(w.etoile3,0,0,500,500);
                        print(230,250,w.temps_final);
                        print(120,150,"Tu es totalement pret pour la guerre");
                    }
                    else if(w.temps_final<22)
                    {
                        image_draw(w.etoile2,0,0,500,500);
                        print(230,250,w.temps_final);
                        print(150,150,"Tu peux mieux faire !");
                    }
                    else if(w.temps_final<30)
                    {
                        image_draw(w.etoile1,0,0,500,500);
                        print(230,250,w.temps_final);
                        print(150,150,"Entraine toi encore !");
                    }
                }
        }
        stop=winDisplay();
    }
    while(!stop);
    winQuit();
    return 0;

}

