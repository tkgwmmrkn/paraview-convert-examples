#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define IN_DIR "."
#define OUT_DIR "./output"

#define STEPS 180   // ステップ数
#define NX 389      // X方向ポイント数
#define NY 6        // Y方向ポイント数
#define NZ 72       // Z方向ポイント数
#define XO 0.0      // 原点のX座標
#define YO 0.0      // 原点のY座標
#define ZO -0.21    // 原点のZ座標
#define DX 0.01     // X格子幅
#define DY 0.06     // Y格子幅
#define DZ 0.015     // Z格子幅

#define POINTS (NX*NY*NZ)

FILE* fp_in;
FILE* fp_zb_inue;
FILE* fp;

float *Value;
float *zb;

int dummy_int;

int num_lines;
int num_xpoints;
int num_ypoints;

bool read_input(){
    float time;
    Value = (float*)malloc(sizeof(float)*POINTS);
    dummy_int=fscanf(fp_in," Time=  %e", &time);
    if (dummy_int == 0 || dummy_int == EOF){
        return false;
    }

    for(int i=0;i<POINTS;i++) {
        float a;
        dummy_int=fscanf(fp_in,"%f ",&a);
        Value[i]=a;
    }


    //////////////// zb_inue_1.dat //////////
    dummy_int=fscanf(fp_zb_inue," title=zb.dat\n VARIABLES=\"x\",\"y\",\"zb\",\n ZONE SolutionTime= %e ,F=POINT,i= %d ,j=\n %d ,", &time, &num_xpoints, &num_ypoints);
    if (dummy_int == 0 || dummy_int == EOF){
        return false;
    }
    zb = (float*)malloc(sizeof(float)*num_xpoints*num_ypoints*3);

    for(int i=0;i<num_xpoints*num_ypoints;i++) {
        float a[3];
        dummy_int=fscanf(fp_zb_inue,"%f %f %f ",&a[0], &a[1], &a[2]);
        zb[3*i]=a[0];
        zb[3*i+1]=a[1];
        zb[3*i+2]=a[2];
    }

    return true;
}

void make_vti(int j) {
    char filename[256];
    sprintf(filename, "%s/output_%05d.vti",OUT_DIR,j);
    printf("Creating %s ... ", filename);

    fp=fopen(filename,"w");
    fprintf(fp,"<?xml version='1.0' encoding='UTF-8'?>\n");
    fprintf(fp,"<VTKFile byte_order='LittleEndian' version='0.1' type='ImageData'>\n");
    fprintf(fp,"<ImageData WholeExtent='0 %d 0 %d 0 %d' Origin='%f %f %f' Spacing='%f %f %f'>\n",NX-1,NY-1,NZ-1,XO,YO,ZO,DX,DY,DZ);
    fprintf(fp,"<Piece Extent='0 %d 0 %d 0 %d'>\n",NX-1,NY-1,NZ-1);

    fprintf(fp,"<PointData>\n");
    fprintf(fp,"<DataArray NumberOfComponents='1' type='Float32' Name='f' format='ascii'>\n");
    for(int i=0;i<POINTS;i++)fprintf(fp,"%lf ",Value[i]);
    fprintf(fp,"\n</DataArray>\n");
    fprintf(fp,"</PointData>\n");

    fprintf(fp,"</Piece>\n");
    fprintf(fp,"</ImageData>\n");
    fprintf(fp,"</VTKFile>\n");

    fclose(fp);
    printf("done.\n");
}

void make_vtp(int j) {
    char filename[256];
    sprintf(filename, "%s/line_%05d.vtp",OUT_DIR,j);
    printf("Creating %s ... ", filename);

    fp=fopen(filename,"w");
    fprintf(fp,"<?xml version='1.0' encoding='UTF-8'?>\n");
    fprintf(fp,"<VTKFile byte_order='LittleEndian' version='0.1' type='PolyData'>\n");
    fprintf(fp,"<PolyData>");
    fprintf(fp,"<Piece NumberOfPoints='%d' NumberOfLines='%d' NumberOfVerts='0' NumberOfStrips='0' NumberOfPolys='0'>", num_xpoints, num_xpoints-1);

    fprintf(fp,"<Points>\n");
    fprintf(fp,"<DataArray NumberOfComponents='3' type='Float32' Name='points' format='ascii'>\n");
    for(int i=0;i<num_xpoints;i++){
        fprintf(fp,"%lf %lf %lf\n",zb[3*i], 0.0, zb[3*i+2]);
    }
    fprintf(fp,"</DataArray>\n");
    fprintf(fp,"</Points>\n");
    fprintf(fp,"<Lines>\n");
    fprintf(fp,"<DataArray type='Int32' Name='connectivity' format='ascii'>\n");
    for(int i=0;i<num_xpoints-1;i++){
        fprintf(fp,"%d %d ",i,i+1);
    }
    fprintf(fp,"\n</DataArray>\n");
    fprintf(fp,"<DataArray type='Int32' Name='offsets' format='ascii'>\n");
    for(int i=1;i<num_xpoints;i++){
        fprintf(fp,"%d ",2*i);
    }
    fprintf(fp,"\n</DataArray>\n");
    fprintf(fp,"</Lines>\n");

    fprintf(fp,"</Piece>\n");
    fprintf(fp,"</PolyData>\n");
    fprintf(fp,"</VTKFile>\n");

    fclose(fp);
    printf("done.\n");
}

int main( int argc, char** argv) {
    fp_in = fopen("hirano_avs.dat", "r");
    fp_zb_inue = fopen("zb_inue_1.dat", "r");
    dummy_int = system("mkdir output");
    for(int j=0;j<STEPS;j++){
        if (read_input()){
            make_vti(j);
            make_vtp(j);

            free(Value);
            free(zb);
        } else {
            printf("File End Detected: l.%d", j-1);
            break;
        }
    }
    printf(" END. \n");
    fclose(fp_in);
    fclose(fp_zb_inue);

    return 0;
}

