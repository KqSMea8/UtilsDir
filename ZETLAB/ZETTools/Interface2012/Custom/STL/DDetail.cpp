#include "stdafx.h"
#include <Interface2012\Custom\STL\DDetail.h>
#include "math.h"

//------------------------------------------------------------------------------
CDDetail::CDDetail()
{
	pVertexs.clear();
	pNormals.clear();
	pTexCoords.clear();
    pMultiTexCoords.clear();
}
//------------------------------------------------------------------------------
CDDetail::~CDDetail()
{
	pVertexs.clear();
	pNormals.clear();
	pTexCoords.clear();
	pMultiTexCoords.clear();
}
//------------------------------------------------------------------------------
void CDDetail::Draw(facet *pFacet, int QntPlanes){
	int Const1(2), Const2(2);
	GLfloat HeightPlane(2.0f), WidthPlane(2.0f), DepthPlane(0.0f);

	static GLfloat rquad(0);
	int Counter(0), Counter1(0);

	glPushMatrix();
	glLoadIdentity();
	glTranslatef(OffsetX, OffsetY, OffsetZ);
	glScalef(ScaleX, ScaleY, ScaleZ);
	glRotatef(Rotate, 0.0f, 0.0f, 1.0f);
	glRotatef(RotateX, 1.0f, 0.0f, 0.0f);
	glTranslatef(-MiddleX, -MiddleY, -MiddleZ);

    glPushAttrib(GL_ENABLE_BIT | GL_LIGHTING_BIT);
    glPushClientAttrib(GL_ENABLE_BIT | GL_LIGHTING_BIT);
    glEnable(GL_LIGHTING);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    glNormalPointer(GL_FLOAT, 0, (void*)(pNormals.data()));
    glVertexPointer(3, GL_FLOAT, 0, (void*)pVertexs.data());

    glDrawArrays(GL_TRIANGLES, 0, QntPlanes*3);

    glDisable(GL_LIGHTING);

    glPopClientAttrib();
    glPopAttrib();
    glPopMatrix();

};
//------------------------------------------------------------------------------
void CDDetail::Draw_texture(facet *pFacet, int QntPlanes, int *PicSize, bool Shift=false){
	int Const1=2, Const2=2;
	GLfloat HeightPlane=2.0f, WidthPlane=2.0f, DepthPlane=0.0f;
	static GLfloat rquad=0;
	int Counter(0), Counter1(0);
	float x(0.f), y(0.f), xa(0.f), ya(0.f), a(0.f);

	glPushMatrix();
	glLoadIdentity();
	glTranslatef(OffsetX, OffsetY, OffsetZ);  
	glScalef(ScaleX, ScaleY, ScaleZ);
	glRotatef(Rotate, 0.0f, 0.0f, 1.0f);
	glRotatef(RotateX, 1.0f, 0.0f, 0.0f);
	glTranslatef(-MiddleX, -MiddleY, -MiddleZ);

	glPushAttrib(GL_ENABLE_BIT | GL_LIGHTING_BIT);
	glPushClientAttrib(GL_ENABLE_BIT | GL_LIGHTING_BIT);
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glNormalPointer(GL_FLOAT, 0, (void*)(pNormals.data()));
	glTexCoordPointer(2, GL_FLOAT, 0, (void*)pTexCoords.data());
	glVertexPointer(3, GL_FLOAT, 0, (void*)pVertexs.data());

	glDrawArrays(GL_TRIANGLES, 0, QntPlanes*3);
	
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);

	glPopClientAttrib();
	glPopAttrib();
	glPopMatrix();
};
//------------------------------------------------------------------------------
void CDDetail::Draw_texture_VBO(GLuint VBO, facet *pFacet, int QntPlanes, int *PicSize, bool Shift=false)
{
    //int Const1=2, Const2=2;
    //GLfloat HeightPlane=2.0f, WidthPlane=2.0f, DepthPlane=0.0f;
    //static GLfloat rquad=0;
    //int Counter, Counter1;
    //float x, y, xa, ya, a;

    //glPushMatrix();
    //glLoadIdentity();
    //glTranslatef(OffsetX, OffsetY, OffsetZ);  
    //glScalef(ScaleX, ScaleY, ScaleZ);
    //glRotatef(Rotate, 0.0f, 0.0f, 1.0f);
    //glRotatef(RotateX, 1.0f, 0.0f, 0.0f);
    //glTranslatef(-MiddleX, -MiddleY, -MiddleZ);

    //glGenBuffers = (PFNGLGENBUFFERSPROC)wglGetProcAddress("glGenBuffers");
    //glBindBuffer = (PFNGLBINDBUFFERPROC)wglGetProcAddress("glBindBuffer");
    //glBufferData = (PFNGLBUFFERDATAPROC)wglGetProcAddress("glBufferData");
    //glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)wglGetProcAddress("glVertexAttribPointer");
    //glGenBuffers(1, &VBO);
    //glBindBuffer(GL_ARRAY_BUFFER, VBO);
    ////! Вершины нашего треугольника
    ////! Передаем вершины в буфер
    //glBufferData(GL_ARRAY_BUFFER, sizeof(pVertexs), pVertexs.data(), GL_STATIC_DRAW);

    ////! Указывая pointer 0 при подключенном буфере, мы указываем что данные в VBO
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    //glDrawArrays(GL_TRIANGLES, 0, QntPlanes*3);

    ////! Отключаем VBO
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    //glDisable(GL_LIGHTING);
    //glDisable(GL_TEXTURE_2D);

    //glPopClientAttrib();
    //glPopAttrib();
    //glPopMatrix();
}
//------------------------------------------------------------------------------
void CDDetail::Draw_HMME(facet *pFacet, int QntPlanes, int *PicSize){
	int Const1=2, Const2=2;
	GLfloat HeightPlane(2.0f), WidthPlane(2.0f), DepthPlane(0.0f);
	GLfloat a[3], b[3];
	static GLfloat rquad(0);
	int Counter(0), Counter1(0);

	float mat_dif[]  = {0.5f, 0.4f, 0.4f};
	float mat_amb[]  = {0.3f, 0.7f, 0.3f};
	float mat_spec[] = {1.0f, 1.0f, 1.0f};
	float shininess = 1.0f * 128;

	glPushMatrix();
	glLoadIdentity();
	glTranslatef(OffsetX, OffsetY, OffsetZ);          // Сдвинуть вглубь экрана
	glScalef(ScaleX, ScaleY, ScaleZ);
	glRotatef(Rotate, 0.0f, 0.0f, 1.0f);
	glRotatef(RotateX, 1.0f, 0.0f, 0.0f);
	glTranslatef(-MiddleX, -MiddleY, -MiddleZ);

	glPushAttrib(GL_ENABLE_BIT | GL_LIGHTING_BIT);
    glPushClientAttrib(GL_ENABLE_BIT | GL_LIGHTING_BIT);
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glNormalPointer(GL_FLOAT, 0, (void*)(pNormals.data()));
    glTexCoordPointer(2, GL_FLOAT, 0, (void*)pTexCoords.data());
    glVertexPointer(3, GL_FLOAT, 0, (void*)pVertexs.data());

    glDrawArrays(GL_TRIANGLES, 0, QntPlanes*3);

    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);

    glPopClientAttrib();
	glPopAttrib();
	glPopMatrix();
};
//------------------------------------------------------------------------------
void CDDetail::Draw_multitexture(facet *pFacet, int QntPlanes)
{
 //   int Const1=2, Const2=2;
	//GLfloat HeightPlane=2.0f, WidthPlane=2.0f, DepthPlane=0.0f;
	//static GLfloat rquad=0;
	//int Counter, Counter1;
 //   float x, y, xa, ya, a;

	//glPushMatrix();
	//glLoadIdentity();
	//glTranslatef(OffsetX, OffsetY, OffsetZ);  
	//glScalef(ScaleX, ScaleY, ScaleZ);
	//glRotatef(Rotate, 0.0f, 0.0f, 1.0f);
	//glRotatef(RotateX, 1.0f, 0.0f, 0.0f);
	//glTranslatef(-MiddleX, -MiddleY, -MiddleZ);

	//glPushAttrib(GL_ENABLE_BIT | GL_LIGHTING_BIT);
	//glPushClientAttrib(GL_ENABLE_BIT | GL_LIGHTING_BIT);
	//glEnable(GL_LIGHTING);
	//glEnable(GL_TEXTURE_2D);
	///*
	//glBegin(GL_TRIANGLES);
	//for(Counter=0; Counter<QntPlanes; Counter++)
 //   {
	//	for(Counter1=0; Counter1<3; Counter1++)
 //       {
	//		glNormal3f(pFacet[Counter].Normal.X, pFacet[Counter].Normal.Y, pFacet[Counter].Normal.Z);

	//		glTexCoord2f(pFacet[Counter].Texture[Counter1].S, pFacet[Counter].Texture[Counter1].T);
 //           
	//		glVertex3f(pFacet[Counter].Vertex[Counter1].X, pFacet[Counter].Vertex[Counter1].Y, pFacet[Counter].Vertex[Counter1].Z);	
	//	}
	//}
	//glEnd();
	//*/
	//
	////glEnable(GL_VERTEX_ARRAY);
	////glEnable(GL_NORMAL_ARRAY);
	////glEnable(GL_TEXTURE_COORD_ARRAY);
	//glEnableClientState(GL_VERTEX_ARRAY);
	//glEnableClientState(GL_NORMAL_ARRAY);
 //   glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	//glNormalPointer(GL_FLOAT, 0, (void*)(pNormals.data()));

 //   glClientActiveTextureARB(GL_TEXTURE0_ARB);
	////glTexCoordPointer(2, GL_FLOAT, 0, (void*)pMultiTexCoords.data());
 //   glTexCoordPointer(2, GL_FLOAT, 0, (void*)pMultiTexCoords.data());
 //   

 //   glClientActiveTextureARB(GL_TEXTURE1_ARB);
 //   //glTexCoordPointer(2, GL_FLOAT, 0, (void*)pTexCoords.data());
 //   glTexCoordPointer(2, GL_FLOAT, 0, (void*)pTexCoords.data());

	//glVertexPointer(3, GL_FLOAT, 0, (void*)pVertexs.data());

	//glDrawArrays(GL_TRIANGLES, 0, QntPlanes*3);
	//
	//glDisable(GL_LIGHTING);
	//glDisable(GL_TEXTURE_2D);

	//glPopClientAttrib();
	//glPopAttrib();
	//glPopMatrix();
}
//------------------------------------------------------------------------------
void CDDetail::SetOffset(float X, float Y, float Z){
	OffsetX=X;
	OffsetY=Y;
	OffsetZ=Z;
}
//------------------------------------------------------------------------------
void CDDetail::GetOffset(float &X, float &Y, float &Z) {
	X = OffsetX;
	Y = OffsetY;
	Z = OffsetZ;
};
//------------------------------------------------------------------------------
void CDDetail::SetScale(float ScaleX, float ScaleY, float ScaleZ){
	this->ScaleX=ScaleX;
	this->ScaleY=ScaleY;
	this->ScaleZ=ScaleZ;
}
//------------------------------------------------------------------------------
void CDDetail::GetScale(float &X, float &Y, float &Z) {
	X = ScaleX;
	Y = ScaleY;
	Z = ScaleZ;
};
//------------------------------------------------------------------------------
void CDDetail::SetDimension(float Width, float Height, float Depth){
	this->Width=Width;
	this->Height=Height;
	this->Depth=Depth;
};
//------------------------------------------------------------------------------
float CDDetail::GetRotate(){
	return Rotate;
};
//------------------------------------------------------------------------------
void CDDetail::SetRotate(float Rotete){
	this->Rotate=Rotete;
};
//------------------------------------------------------------------------------
void CDDetail::SetRotateX(float RoteteX){
	this->RotateX=RoteteX;
};
//------------------------------------------------------------------------------
void CDDetail::Clear()
{
	pVertexs.clear();
	pNormals.clear();
	pTexCoords.clear();
	if(!pMultiTexCoords.empty())
		pMultiTexCoords.clear();
}
//------------------------------------------------------------------------------
void CDDetail::CalcDetailArray(facet *pFacet, int QntPlanes)
{
	int size3 = QntPlanes * 3 * 3;
	int size2 = QntPlanes * 3 * 2;
	//pVertexs = new float[size3];
	//pNormals = new float[size3];
	//pTexCoords = new float[size2];
	pVertexs.clear();
	pNormals.clear();
	pTexCoords.clear();
	pVertexs.resize(size3);
	pNormals.resize(size3);
	pTexCoords.resize(size2);
	for (int i = 0; i < QntPlanes; i++)
	{
		memcpy((void*)(&pVertexs[i*9]),   (void*)(&pFacet[i].Vertex),  9*sizeof(float));
		memcpy((void*)(&pNormals[i*9+0]), (void*)(&pFacet[i].Normal),  3*sizeof(float));
		memcpy((void*)(&pNormals[i*9+3]), (void*)(&pFacet[i].Normal),  3*sizeof(float));
		memcpy((void*)(&pNormals[i*9+6]), (void*)(&pFacet[i].Normal),  3*sizeof(float));
		memcpy((void*)(&pTexCoords[i*6]), (void*)(&pFacet[i].Texture), 6*sizeof(float));
	}
}
//------------------------------------------------------------------------------
void CDDetail::CalcMultiTextArray(facet *pFacet, int QntPlanes)
{
    int size2 = QntPlanes * 3 * 2;

    pMultiTexCoords.resize(size2);

    for (int i = 0; i < QntPlanes; i++)
    {
        memcpy((void*)(&pMultiTexCoords[i*6]), (void*)(&pFacet[i].Texture), 6*sizeof(float));
    }
}
//------------------------------------------------------------------------------
