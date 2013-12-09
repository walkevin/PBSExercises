/* fscanf example */
#include <stdio.h>

int main ()
{
  FILE * objFile;

  float xx,xy,yx,yy,zx,zy;
  char ch, cha, chb, chc;

  objFile = fopen ("data/cylinder-temp.obj","r+");
  rewind (objFile);
	fscanf (objFile,"%c %f %[/] %f %f %[/] %f %f %[/] %f",&ch,&xx,&cha,&xy,&yx,&chb,&yy,&zx,&chc,&zy);
  fclose (objFile);
  printf ("I have read: %f, %f, %f\n",xx,yx,zx);
  return 0;
}
//  char str [80];
//  float f;
//  FILE * pFile;

//  pFile = fopen ("myfile.txt","w+");
//  fprintf (pFile, "%f %s", 3.1416, "PI");
//  fscanf (pFile, "%f", &f);
//  fscanf (pFile, "%s", str);
//
//  rewind (pFile);

//  fclose (pFile);
//  printf ("I have read: %f and %s \n",f,str);
