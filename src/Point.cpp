#include "Point.h"

using namespace std;

//Malha--------------------------------------------------------------
Malha::Malha(Point &P){
    //VANT in (x,y,z) = (0,0,0)
    int c = 0; //counter
    N_lat = 2;
    N_lon = 2;
    //N_H = 0;

    P.set_x(0);
    P.set_y(0);
    P.set_z(0);

    for(int k = 0; k<= 0; k++){
        for(int j = -N_lon; j<=N_lon; j++){
            for(int i = -N_lat; i<=N_lat;i++){
                Point aux(P.get_latitude() - (i*0.000135),P.get_longitude() - (j*0.000135), P.get_height() + 15*k);
                aux.set_x(i*15);
                aux.set_y(j*15);
                aux.set_z(k*15);
                c = c+1;
                malha.push_back(aux);
            }
        }
    }
    N_Points = c;
}

void Malha::print_malha(){
    for(int i = 0; i<malha.size();i++){
        cout.precision(9);
        print_point_malha(i);
        cout<<endl;
    }
}

void Malha::print_malha_xyz(){
    for(int i = 0; i<malha.size();i++){
        print_point_malha_xyz(i);
        cout<<endl;
    }
}


//Functions--------------------------------------------------------------------------
 double D_to_RAD(double D_x){
        double R_x;
        R_x = D_x*(M_PI/180);
        return(R_x);
    }

double DELTA(double start, double destiny){
        double delta;
        delta = start - destiny;
        delta = sqrt(pow(delta,2));
        return (delta);
    }

double distance_two_points(double lat1, double lat2, double delta_lon, double delta_lat, double radius){
        double a, b, dist;

        a = (pow(sin((delta_lat/2)),2)) + cos(lat1)*(cos(lat2))*(pow(sin((delta_lon/2)), 2));
        b = 2 * (atan2(sqrt(a), sqrt(1-a)));
        dist = radius*b;
        return (dist);
    }

double distance_between_two_points(double D_lat, double D_lon, double D_lat_destiny, double D_lon_destiny){

        double R_lat, R_lon, R_lat_destiny, R_lon_destiny, delta_lat, delta_lon, distance, R_polar, R_equat, e, R;

        //transforming all degrees to radians
        R_lon = D_to_RAD(D_lon);
        R_lat = D_to_RAD(D_lat);
        R_lat_destiny = D_to_RAD(D_lat_destiny);
        R_lon_destiny = D_to_RAD(D_lon_destiny);

        //calculating delta
        delta_lat = DELTA(R_lat, R_lat_destiny);
        delta_lon = DELTA(R_lon, R_lon_destiny);

        //calculating radius
        R = (EARTH_R - (21.385*sin(D_lat)));

        //calculating distance
        distance = distance_two_points(R_lat, R_lat_destiny, delta_lon, delta_lat, R);

        return distance;
}

//Ṕoints-----------------------------------------------------------
double Point::distance_from(Point &p){

        return(distance_between_two_points(this->get_latitude(), this->get_longitude(), p.get_latitude(), p.get_longitude()));
}
double Point::distance_from(double Dlat, double Dlon){

        return(distance_between_two_points(this->get_latitude(), this->get_longitude(), Dlat, Dlon));
}

//Transform function----------------------------------------------------------------------
//talvez essa funcao seja excluida ja que nao está fazendo mt coisa, colocarei tudo no pathfinding.h
//double transf(double lat, double lon, double h, double r, double lat_o, double lon_o){
Malha transf(double lat, double lon, double h){

    //falta colocar parte dos obstaculos
    Point P(lat,lon, h);
    Malha M(P);

    //Funcao do iure chamaria algo como:
    //Point D = a_star(M.get_malha);
    //mas é melhor o pathfinding chamar as funcoes daqui, e nao o contrario.

    return M;
}

//obstacles----------------------------------------------------------------------------------

//PRECISA DE MAIS TESTES
void add_obs (Malha &M, obstacle &O){
    //double length = 2*O.get_radius();
    // 1 degree = 111,139 m or 1m = 1/111139 degrees
    double lat_i, lat_f, lon_i, lon_f;

    lat_i = O.get_obs_lat() - (O.get_radius()*(1/111139));
    lat_f = O.get_obs_lat() + (O.get_radius()*(1/111139));
    lon_i = O.get_obs_lon() - (O.get_radius()*(1/111139));
    lon_f = O.get_obs_lon() + (O.get_radius()*(1/111139));

    for(int i = 0; i<M.malha.size();i++){
        if(M.malha[i].get_latitude() >= lat_i && M.malha[i].get_latitude()<= lat_f){
                if(M.malha[i].get_longitude() >= lon_i && M.malha[i].get_longitude() <= lon_f){
                    if(M.malha[i].get_height() <= O.get_obs_height()){
                         M.malha[i].set_obs_true();
                    }
                }
            }
        }

}
