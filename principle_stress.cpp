#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define IN_DIR "./output"
#define OUT_DIR "./converted"

FILE* fp[4];
char filename[256];
int NumberOfParticle;

double *Stress;
double *Coord;

int istep;
float time;
float time_count;

int dummy_int;
char filenames[4][256] = {"sigx.dat", "sigy.dat", "sigz.dat", "sigxy.dat"};

void mkvtu(int j) {
	sprintf(filename, "%s/stress%d.vtu",OUT_DIR,j);
    printf("Creating %s ... ", filename);

	FILE *fpvtu;
	fpvtu=fopen(filename,"w");
	fprintf(fpvtu,"<?xml version='1.0' encoding='UTF-8'?>\n");
	fprintf(fpvtu,"<VTKFile xmlns='VTK' byte_order='LittleEndian' version='0.1' type='UnstructuredGrid'>\n");
	fprintf(fpvtu,"<UnstructuredGrid>\n");
 	fprintf(fpvtu,"<Piece NumberOfCells='%d' NumberOfPoints='%d'>\n",NumberOfParticle,NumberOfParticle);

 	fprintf(fpvtu,"<Points>\n");
 	fprintf(fpvtu,"<DataArray NumberOfComponents='3' type='Float32' Name='Position' format='ascii'>\n");
	for(int i=0;i<NumberOfParticle;i++){
        fprintf(fpvtu,"%lf %lf %lf\n",Coord[i*2],Coord[i*2+1],0.0);
    }
 	fprintf(fpvtu,"</DataArray>\n");
  	fprintf(fpvtu,"</Points>\n");

  	fprintf(fpvtu,"<PointData>\n");
 	fprintf(fpvtu,"<DataArray NumberOfComponents='3' type='Float32' Name='principal_stress_1' format='ascii'>\n");
	for(int i=0;i<NumberOfParticle;i++){
        if (i > 3536){
            fprintf(fpvtu,"%lf %lf %lf\n", 0.0, 0.0, 0.0);
        } else {
            double sigx  = Stress[                   i];
            double sigy  = Stress[  NumberOfParticle+i];
            double sigz  = Stress[2*NumberOfParticle+i];
            double sigxy = Stress[3*NumberOfParticle+i];
            if (sigxy < 0.001){
                if (sigx > sigy){
                    fprintf(fpvtu,"%lf %lf %lf\n", sigx, 0.0, 0.0);
                } else {
                    fprintf(fpvtu,"%lf %lf %lf\n", 0.0, sigy, 0.0);
                }
            } else {
                double X = sigy - sigx;
                double Y = 2.0*sigxy;
                double pre = ((sigx+sigy)/sqrt(X*X+Y*Y)+1.0)/2.0/sqrt(2.0);
                double root = sqrt(sqrt(X*X+Y*Y)+X);
                double vec1x = pre*Y/root;
                double vec1y = pre*root;
                fprintf(fpvtu,"%lf %lf %lf\n", vec1x, vec1y, 0.0);
            }
        }
    }
 	fprintf(fpvtu,"</DataArray>\n");
 	fprintf(fpvtu,"<DataArray NumberOfComponents='3' type='Float32' Name='principal_stress_2' format='ascii'>\n");
	for(int i=0;i<NumberOfParticle;i++){
        if (i > 3536){
            fprintf(fpvtu,"%lf %lf %lf\n", 0.0, 0.0, 0.0);
        } else {
            double sigx  = Stress[                   i];
            double sigy  = Stress[  NumberOfParticle+i];
            double sigz  = Stress[2*NumberOfParticle+i];
            double sigxy = Stress[3*NumberOfParticle+i];
            if (sigxy < 0.001){
                if (sigx > sigy){
                    fprintf(fpvtu,"%lf %lf %lf\n", sigx, 0.0, 0.0);
                } else {
                    fprintf(fpvtu,"%lf %lf %lf\n", 0.0, sigy, 0.0);
                }
            } else {
                double X = sigy - sigx;
                double Y = 2.0*sigxy;
                double pre = ((sigx+sigy)/sqrt(X*X+Y*Y)-1.0)/2.0/sqrt(2.0);
                double root = sqrt(sqrt(X*X+Y*Y)-X);
                if (sqrt(X*X+Y*Y)-X<0.0){
                    printf("%f %f %f",X, Y, sqrt(X*X+Y*Y));
                    root=0.0;
                }
                double vec1x = pre*Y/root;
                double vec1y = -pre*root;
                fprintf(fpvtu,"%lf %lf %lf\n", vec1x, vec1y, 0.0);
            }
        }
    }
 	fprintf(fpvtu,"</DataArray>\n");
  	fprintf(fpvtu,"</PointData>\n");

 	fprintf(fpvtu,"<Cells>\n");
 	fprintf(fpvtu,"<DataArray type='Int32' Name='connectivity' format='ascii'>\n");
	for(int i=0;i<NumberOfParticle;i++)fprintf(fpvtu,"%d\n",i);
 	fprintf(fpvtu,"</DataArray>\n");
 	fprintf(fpvtu,"<DataArray type='Int32' Name='offsets' format='ascii'>\n");
	for(int i=0;i<NumberOfParticle;i++)fprintf(fpvtu,"%d\n",i+1);
 	fprintf(fpvtu,"</DataArray>\n");
 	fprintf(fpvtu,"<DataArray type='UInt8' Name='types' format='ascii'>\n");
	for(int i=0;i<NumberOfParticle;i++)fprintf(fpvtu,"1\n");
 	fprintf(fpvtu,"</DataArray>\n");
 	fprintf(fpvtu,"</Cells>\n");

 	fprintf(fpvtu,"</Piece>\n");
 	fprintf(fpvtu,"</UnstructuredGrid>\n");
 	fprintf(fpvtu,"</VTKFile>\n");

	fclose(fpvtu);
	printf("done.\n");
}

int main( int argc, char** argv) {
    FILE* fp[4];
    bool exit = false;
    char command[256];
	sprintf(command, "mkdir \"%s\"",OUT_DIR);   // make output directory
    dummy_int = system(command);

    time_count = 0;
    while (true) {
        if (time_count == 0){
            for(int iFile=0;iFile<4;iFile++){
                sprintf(filename, "%s/%s",IN_DIR,filenames[iFile]);
                fp[iFile] = fopen(filename, "r");
                printf("reading %s...\n", filename);
                if (iFile == 0) {
                    int particles = 0;
                    dummy_int=fscanf(fp[iFile],"%*[^\n]\n"); // skip 1 line
                    while (true) {
                        float a[3];
                        int b;
                        dummy_int=fscanf(fp[iFile]," %d %f %f %f",&b,&a[0],&a[1],&a[2]);
                        if (dummy_int == 0 || dummy_int == EOF) {
                            break;
                        } else {
                            particles++;
                        }
                    }
                    NumberOfParticle = particles-1;
                    printf("NumberOfParticle: %d\n",NumberOfParticle);

                    Stress = (double*)malloc(sizeof(double)*NumberOfParticle*4);
                    Coord = (double*)malloc(sizeof(double)*NumberOfParticle*2);

                    rewind(fp[iFile]);
                }
            }
        }

        for(int iFile=0;iFile<4;iFile++){
            dummy_int=fscanf(fp[iFile]," %d # time= %f",&istep, &time);
            if (dummy_int == 0 || dummy_int == EOF){
	            printf(" FileEnd detected. \n");
                exit = true;
                break;
            }
            for(int i=0;i<NumberOfParticle;i++) {
                float a[3];
                int b;
                dummy_int=fscanf(fp[iFile]," %d %e %e %e",&b,&a[0],&a[1],&a[2]);
                Stress[iFile*NumberOfParticle+i] = a[0];
                if (iFile == 0) {
                    Coord[2*i] = a[1];
                    Coord[2*i+1] = a[2];
                }
            }
        }

        if (exit) {
            break;
        }

        mkvtu(time_count);

        time_count++;
    }

	free(Stress);
	free(Coord);

	printf(" END. \n");

	return 0;
}

