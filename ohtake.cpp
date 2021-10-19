#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define IN_DIR "." //input dir
#define OUT_DIR "particles" //output dir

/*
 * This program converts CSV file incuding XYZ and 1 value
 *   into Paraview particles file (.particles).
 * This program is nice because number of points is automatically calculated.
 * 
 * Yudai Takegawa 2021 Oct 19
 *  tested compiler : g++ - gcc C++ compiler
 */

float *Area;    // "area.csv" Float32 data array
float *Point;   // "point.csv" Float32 data array
int dummy_int;  // I/O status integer
int area_count = 0;     // "area.csv" data count
int point_count = 0;    // "point.csv" data count

void init(){
    char command[256];
    sprintf(command, "mkdir \"%s\"",OUT_DIR);   // make output directory
    dummy_int = system(command);
    sprintf(command, "mkdir \"%s\\animation\"",OUT_DIR);    // make directory for saving animation
    dummy_int = system(command);
}

void read_area(FILE* fp){
    dummy_int=fscanf(fp,"%*[^\n]\n");   //skip 1 line

    while (true) {
        float a[5];
        dummy_int=fscanf(fp,"%f,%f,%f,%f,%f",&a[0],&a[1],&a[2],&a[3],&a[4]);
        if (dummy_int == 0 || dummy_int == EOF) {
            break;
        }
        area_count++;   //calculate the number of areas
    }
    Area = (float*)malloc(sizeof(float)*area_count*5);  //allocate memory

    rewind(fp); //re-read the CSV file
    
    dummy_int=fscanf(fp,"%*[^\n]\n");   //skip header line
    for (int j = 0; j < area_count; j++) {
        float a[5];
        dummy_int=fscanf(fp,"%f,%f,%f,%f,%f",&a[0],&a[1],&a[2],&a[3],&a[4]);
        //put values
        Area[5*j  ] = a[0];
        Area[5*j+1] = a[1];
        Area[5*j+2] = a[2];
        Area[5*j+3] = a[3];
        Area[5*j+4] = a[4];
    }
}

void read_point(FILE* fp){
    dummy_int=fscanf(fp,"%*[^\n]\n");

    while (true) {
        float a[5];
        dummy_int=fscanf(fp,"%f,%f,%f,%f,%f",&a[0],&a[1],&a[2],&a[3],&a[4]);
        if (dummy_int == 0 || dummy_int == EOF) {
            break;
        }
        point_count++;
    }
    Point = (float*)malloc(sizeof(float)*point_count*5);
    rewind(fp);
    dummy_int=fscanf(fp,"%*[^\n]\n");
    for (int j = 0; j < point_count; j++) {
        float a[5];
        dummy_int=fscanf(fp,"%f,%f,%f,%f,%f",&a[0],&a[1],&a[2],&a[3],&a[4]);
        Point[5*j  ] = a[0];
        Point[5*j+1] = a[1];
        Point[5*j+2] = a[2];
        Point[5*j+3] = a[3];
        Point[5*j+4] = a[4];
    }
}

void make_particles() {
    FILE* fp;
    char filename[256];
    //////////////////
    sprintf(filename, "%s/area.particles",OUT_DIR);
    printf("Creating %s ... ", filename);

    fp=fopen(filename,"w");
    for(int i=0;i<area_count;i++){
        fprintf(fp,"%f %f %f %f\n", Area[5*i],Area[5*i+1],Area[5*i+2],Area[5*i+4]);
    }
    fclose(fp);
    printf("done.\n");
    //////////////////
    sprintf(filename, "%s/points.particles",OUT_DIR);
    printf("Creating %s ... ", filename);

    fp=fopen(filename,"w");
    for(int i=0;i<point_count;i++){
        fprintf(fp,"%f %f %f %f %f\n",Point[5*i],Point[5*i+1],Point[5*i+2],Point[5*i+3]);
    }
    fclose(fp);
    printf("done.\n");
}

int main( int argc, char** argv) {
    init();

    FILE* fp_area_csv = fopen("data1_area.csv", "r");
    FILE* fp_point_csv = fopen("data1_point.csv", "r");

    read_area(fp_area_csv);
    read_point(fp_point_csv);
    make_particles();

    free(Area);
    free(Point);
    printf(" END. \n");
    fclose(fp_area_csv);
    fclose(fp_point_csv);

    return 0;
}

