#include<iostream>
#include<random>
using namespace std;

double randomDouble() {
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0, 1);
    return dis(gen);
}

const int numParticles = 20;
const int maxIterations = 500;
const int tasks = 10;
const double timetask[tasks] = {10,300,165,98,88,113,135,99,80,45};
const int vms = 7;
const double mips[vms] = {8, 1, 4, 6, 5, 3, 4};
double gbest [tasks]; // best particle
double upper_band=vms-1, lower_band=0;


int customRound(double x){
    int temp = int(x);
    double rem = x - temp;
    rem *=10;
    if ( rem >= 5 )
       if(temp + 1 == vms) return temp;
       else return temp +1;
    return temp;
}

double findMax(double arr[], int size) {
    double maxVal = arr[0];
    for (int i = 1; i < size; ++i) {
        if (arr[i] > maxVal) {
            maxVal = arr[i]; 
        } 
    }
    return maxVal;
}

// for initial v
double generateRandom(){
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> distrib(0.0, vms-1); 
    double x = distrib(gen);
    return x;
}


class particle{
    public:
        double v, w, c1, c2;
        double pbest [tasks];
        double bestfitness;
        double x [tasks];

        particle(){
            c1 = 2;c2 = 2;
            w = 0.9;
            bestfitness = 0.0;
            v = generateRandom();
            for(int i=0; i<tasks ; i++)
                x[i] = generateRandom();
        }
        void updateW(double t);
        void updateParticle();
};


double Fitness(particle& p) {

    const double w1 = 0.5, w2 = 0.5;
    double s = 0.0, seconds = 0.0, makespan;

    double busy [vms];

    for(int i=0; i < vms; i++){
        s = 0.0;
        for (int j=0; j < tasks; j++){
            if ( customRound((p.x[j])) == i ) 
                s += timetask[j] / mips[i];
        }
        seconds += s;
        busy[i] = s;
    }

    makespan = findMax(busy, vms);

    double avg = seconds / vms;
    double loadBalancing = makespan / avg;

    double fit =  w1 * ( 1.0 / makespan )+ w2 * (1.0 / loadBalancing);
    // double fit = 1.0/makespan;

    // update pbest 
    if (fit > p.bestfitness){
        p.bestfitness = fit;
        for (int i=0; i<tasks; i++)
            p.pbest[i] = p.x[i];
    } 
    return fit;
}


void particle::updateParticle() {
   for (int i=0; i < tasks ; i++){
    this->v = (this->w * this->v) + (this->c1 * randomDouble() *(this->pbest[i] - this->x[i])) + (this->c2 * randomDouble() *(gbest[i] - this->x[i]));
    this->x[i] += this->v;

    if( this->x[i] < 0 ) this->x[i] = lower_band ;
    else if ( this->x[i] > vms-1 ) this->x[i] = upper_band;
   }
}


void particle::updateW(double t){
    const double w_max = 0.9, w_min = 0.4;
    this-> w = w_max - ((w_max - w_min) * t  / double(maxIterations));
}

int main() {

    particle population [numParticles];

    double fitgbest = 0.0, f;

    for (int k=0; k < maxIterations ; k++){
        for (int i=0; i < numParticles; i++)
        { 
            f = Fitness(population[i]);
                if(f > fitgbest ){
                    fitgbest = f;
                    for (int j=0; j<tasks; j++)
                        gbest[j] = population[i].x[j];
                }
            population[i].updateW(double(k));
            population[i].updateParticle();
        }

    }

    // Show best particle 
        // cout<<"\nthe best particle: \n";
        // for(int i=0; i<tasks ; i++)
        //     cout<<gbest[i]<<"  ";

    // Show output list
        // cout<<"\n\n";
        // for(int i=0; i<tasks ; i++)
        //     cout<<"Task "<<i+1<<" ("<<timetask[i]<<") :  VM(" <<customRound(gbest[i])+1<<")  ->  "<<mips[customRound(gbest[i])]<<endl;

    cout<<"\nBest fitness = "<<fitgbest;

    return 0;
}
