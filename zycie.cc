#include <iostream>
#include <random>
#include <cstdlib>
#include <unistd.h>
#include <fstream>
#include <string>

using namespace std;

class plansza
{
    int hig, wig;
    bool ** T;
public:
    plansza(int fh, int fw);
    void setF();
    bool check(int k, int l);
    void setRandom( double prob);
    bool get(int x, int y);
    void set(int x, int y, bool ans);
    bool checkAndMiracle(int k, int l, double DC);
};

void printToFile( plansza gra, string file, int H, int S ){
	fstream str;
	str.open( file.c_str(), ios::out);
	for( int i = 1; i <= H ; i++){
		for( int j = 1; j <= S ; j++){
			str << gra.get(i,j) << ' ';
		}
		str << endl;
	}
	str.close();
}


bool plansza::checkAndMiracle(int k, int l, double DC){
    int suma = 0;
    for( int i = k-1; i < k+2;i++){
        for(int j = l - 1; j < l + 2; j++ ){
            if( T[i][j] == true) suma = suma + 1;
        }
    }
    suma -= T[k][l];
//    if(DC > 0.0105) cout << "K = " << k << "; L = " << l << "; DC = " << DC << "; wynik = " << (suma > 2 || DC > 0.0105) <<"; ";    
    return( (DC > 0.0108) || (suma > 2) );
}

void plansza::setRandom( double prob ){
    default_random_engine generator;
    int seed_set = static_cast<int>(time(0)) ;
    generator.seed( seed_set );
    bernoulli_distribution distribution( prob );
    for(int i = 1; i <= hig  ; i++){
        for(int j = 1; j <= wig ; j++) T[i][j] = distribution(generator);
    }
}

void plansza::set(int x, int y, bool ans){
    T[x][y] = ans;
}

bool plansza::get(int x, int y){
    bool tmp = T[x][y];
    return(tmp);
}

void show(int H, int S, plansza gra){
    cout << "============================" << endl;
    for(int i = 0; i <= H+1; i++){
        for(int j = 0; j <=  S+1; j++){
            if( (i == 0 && j <= S+1) || (i == H+1 && j <= S+1) || (i <= H+1 && j == S+1) || (i <= H+1 && j == 0) ){
                cout << false;
            }else{
                cout << gra.get(i,j);
            }
        }
	cout << endl;
    }
	cout << "============================" << endl;
}

void plansza::setF()
{
    for( int i = 0; i < hig + 2;i++){
        for( int j = 0; j < wig +2; j++){
            T[i][j] = false;
        }
    }
}

bool plansza::check(int k, int l){
    int suma = 0;
    for( int i = k-1; i < k+2;i++){
        for(int j = l - 1; j < l + 2; j++ ){
            if( T[i][j] == true) suma = suma + 1;
        }
    }
    suma -= T[k][l];
    return(suma > 2);
}

plansza::plansza( int fh, int fw)
    : hig(fh), wig(fw)
{
    T = new bool * [hig+1];
    for( int i = 0; i < hig+2 ; i++){
        T[i] = new bool [wig+1];
    }
}

void nextStep( plansza & tmp, plansza & start, int H, int S){
    for(int i = 1; i <= H; i++){
        for( int j = 1; j <= S; j++){
            tmp.set( i , j, start.check(i,j) );
        }
    }
}

void nextStepMiricol( plansza & tmp, plansza & start, int H, int S, double &DC ){
	for( int i = 1; i <= H; i++){
		for(int j = 1; j <= S; j++){
			DC = ((double) rand() / RAND_MAX) * 0.011;
			tmp.set(i, j, start.checkAndMiracle(i,j, DC));
		}
	}
    show(H, S, tmp);
}



int main( int argc , char * argv[]){

    if( argc != 5){
	cout << "Program ŻYCIE, potrzebuje czterech argumentów:" << endl <<
		 "1) Wysokość planszy" << endl << "2) Szerokość planszy" << endl <<
		 "3) Prawdopodobieństwo losowania planszy startowej" <<
		 "4) Liczbę iteracji kolejnych etapów życia" << endl << "np. zycie 30 60 0.15 150" << endl;
		exit(1);
    }

    int H = strtol(argv[1], NULL, 10);
    int S = strtol(argv[2], NULL, 10);
    double prob = strtod(argv[3], NULL);
    int iter = strtol(argv[4], NULL, 10);
    double DC;

    plansza gra(H, S);
    gra.setF();
    
    plansza tmp(H,S);
    tmp.setF();

    gra.setRandom( prob );
    string what = "gnuplot -e \"set pal def (0 'white', 1 'black'); set terminal png;set output 'data";
//    show(H, S, gra);
    printToFile(gra, "dane.dat", H, S);
    string tmp_str;
    tmp_str = what + "000.png'; plot 'dane.dat' matrix with image\"";
    system(tmp_str.c_str());
    for(int i=0;i < iter; i++){
	    cout << "i = " << i <<" "  << i % 2 <<  endl;

	   if( i % 2){
        if(i < 10){
            tmp_str = what + "00" + to_string(i)  +".png' ; plot 'dane.dat' matrix with image\"";
        }else{
            if(i < 100){
                tmp_str = what + "0" + to_string(i)  +".png' ; plot 'dane.dat' matrix with image\"";
            }else{
                tmp_str = what + to_string(i)  +".png' ; plot 'dane.dat' matrix with image\"";
            }
        }
		nextStep(gra, tmp, H, S);
		printToFile(tmp,"dane.dat",H,S);
        system(tmp_str.c_str());
//		show(H, S, gra);
	   }else{
        if(i < 10){
            tmp_str = what + "00" + to_string(i)  +".png' ; plot 'dane.dat' matrix with image \"";
        }else{
            if(i < 100){
                tmp_str = what + "0" + to_string(i)  +".png' ; plot 'dane.dat' matrix with image\"";
            }else{
                tmp_str = what + to_string(i)  +".png' ; plot 'dane.dat' matrix with image\"";
            }
        }

		nextStepMiricol(tmp, gra, H, S, DC);
//        nextStep(tmp, gra, H, S);
		printToFile(gra,"dane.dat", H,S);
        system(tmp_str.c_str());
//		show(H, S, tmp);
	   }
	   usleep(200000);
    }

return 0;
}



