# Computer-Graphics-OenGL
This repository is code for "Computer Graphics" Project (Modeling, Rendering) using OpenGL library (freeglut-3.2.1), C++ (Visual Studio 2019).

:bulb: **First: Opne Computer Graphics.pdf**<BR/><BR/>
:bulb: **Second: Opne Run Project->20192099_Limhyunmin.exe**

:small_orange_diamond: **savemodel.cpp Characteristics**
- Rotate the entered points on a specific center axis.
- Use OpenGL to express the points that have been rotated on the screen. 
- Store in txt format using SOR modeling data storage function.<BR/><BR/>

:small_blue_diamond: **c++ code - SOR modeling data storage function**
<pre>
<code>
void SaveModel()
{
	FILE* fout;
	fopen_s(&fout, "c:\\data\\myModel.dat", "w");
	fprintf(fout, "VERTEX = %d\n", pnum+2);
	for (int i = 0; i < pnum; i++)
	{
		fprintf(fout, "%.1f %.1f %.1f\n", arRotPoints[i].x, arRotPoints[i].y, arRotPoints[i].z);
	}
	fprintf(fout, "0.0 %.1f 0.0\n", arRotPoints[pnum].y);
	fprintf(fout, "0.0 %.1f 0.0\n", arRotPoints[pnum+count-1].y);

	

	fprintf(fout, "FACE = %d\n", fnum);
	for (int i = 0; i < fnum; i++)
	{
		fprintf(fout, "%d %d %d\n", arFaces[i].ip[0], arFaces[i].ip[1], arFaces[i].ip[2]);
	}
	fprintf(fout, "VERTEX = 1 \n");
	for (int i = 0; i < 1; i++)
	{
		if (i = count - 1)
			fprintf(fout, "%.1f\n", arRotPoints[i].y);

	}
	fclose(fout);
}
</code>
</pre>

:small_orange_diamond: **practice1.cpp Characteristics**
- Load stored sor model into virtual space.
- Shading is implemented by adding lighting.
- various of Rendering styles(WireFrame, Flat Shading, Smooth Shading).
- BackFaceCulling function On/Off.
- Move and time point conversion.
- Transparent (parallel/ perspective).
- Change color using R,G,B.
- Movement and rotation.<BR/><BR/>
