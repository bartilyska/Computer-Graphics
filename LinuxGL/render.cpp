#define GL_GLEXT_PROTOTYPES

#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include "render.h"
#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtc/type_ptr.hpp"

const int ilevbo=8;
const int ilevao=4;
const int ileebo=2;

GLuint vbo[ilevbo];		//identyfikatory buforow wierzcholkow
GLuint vao[ilevao];		//identyfikatory tablic wierzcholkow
GLuint ebo[ileebo];		//identyfikator bufora elementow

GLuint shaderProgram;
GLint vertexShader;	//identyfikator shadera wierzcholkow
GLint fragmentShader;   //identyfikator shadera fragmentow
GLint posAttrib, colAttrib;	//

glm::mat4 viewMatrix = glm::mat4();
glm::mat4 projectionMatrix = glm::mat4(); //marzerz widoku i rzutowania
GLfloat fi = 0;
GLfloat R = 1.0f;

//-------------Atrybuty wierzcholkow------------------------------------------

	GLfloat ver_triangle[] = {	//wspolrzedne wierzcholkow trojkata
		 0.0f,  1.0f, 0.0,
		 1.0f,  0.0f, 0.0,
		-1.0f,  0.0f, 0.0
	};

	GLfloat col_triangle[] = {	//kolory wierzcholkow trojkata
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
	};

	GLfloat ver_rectangle[] = {	//wspolrzedne wierzcholkow prostokata
		-1.0f, -0.2f, 0.0f,
		 1.0f, -0.2f, 0.0f,
		-1.0f, -0.7f, 0.0f,
		 1.0f, -0.7f, 0.0f
	};

	GLfloat col_rectangle[] = {	//kolory wierzcholkow prostokata
		0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f
	};

	GLuint elements[] = { //prostokat skladamy z dwoch trojkatow
		0, 1, 2,		  //indeksy wierzcholkow dla pierwszego trojkata
		1, 2, 3,			  //indeksy wierzcholkow dla drugiego trojkata
	};
  
GLfloat ver_prostopadloscian[] = {
    -0.2f, -0.2f, -0.2f,
     0.2f, -0.2f, -0.2f,
     0.2f,  0.2f, -0.2f,
    -0.2f,  0.2f, -0.2f,
    -0.2f, -0.2f,  0.2f,
     0.2f, -0.2f,  0.2f,
     0.2f,  0.2f,  0.2f,
    -0.2f,  0.2f,  0.2f
};
GLfloat col_prostopadloscian[] = {	//kolory wierzcholkow prostopadloscianu
		1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 0.0f,
    1.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 0.0f
	};
	GLuint prostopadloscianElements[] = { 
    0, 1, 2, 0, 2, 3, 
    4, 5, 6, 4, 6, 7, 
    0, 4, 7, 0, 7, 3, 
    1, 5, 6, 1, 6, 2,
    3, 2, 6, 3, 6, 7,
    0, 1, 5, 0, 5, 4  
};
//wyznacz wspolrzedne na okregu
GLfloat ver_walec[3*26] = {	//wspolrzedne wierzcholkow walca
	};
GLfloat col_walec[3*26] = {	//kolory wierzcholkow walca
	1.0f, 0.0f, 0.0f,//1
    0.0f, 1.0f, 0.0f,//2
    0.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 0.0f,
    1.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 0.0f,
    1.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 0.0f,
    1.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 0.0f,
    	1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
  };

//----------------------------kod shadera wierzcholkow-----------------------------------------

const GLchar* vShader_string =
{
  "#version 130\n"\

  "in vec3 position;\n"\
  "in vec3 color;\n"\
  "out vec3 Color;\n"\
  "uniform mat4 transformMatrix;\n"\
  "void main(void)\n"\
  "{\n"\
  "  gl_Position = transformMatrix * vec4(position, 1.0);\n"\
  "  Color = color;\n"\
  "}\n"
};

//----------------------------kod shadera fragmentow-------------------------------------------
const GLchar* fShader_string =
{
  "#version 130\n"\
  "in  vec3 Color;\n"\
  "out vec4 outColor;\n"\

  "void main(void)\n"\
  "{\n"\
  "  outColor = vec4(Color, 1.0);\n"\
  "}\n"
};


//------------------------------------------------zmiana rozmiaru okna---------------------------

void resizeGLScene(unsigned int width, unsigned int height)
{
    if (height == 0) height = 1; // zabezpiecznie dla okna o zerowej wysokosci
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION); 
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 1.0f, 500.0f);
    glMatrixMode(GL_MODELVIEW);
}


//----------------------------------tworzenie, wczytanie, kompilacja shaderow-------------------------

int initShaders(void)
{
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vShader_string, NULL);
    glCompileShader(vertexShader);
    
    GLint status;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
    if (status == GL_TRUE)
      std::cout << "Kompilacja shadera wierzcholkow powiodla sie!\n";
    else
    {
      std::cout << "Kompilacja shadera wierzcholkow NIE powiodla sie!\n";
      return 0;
     }
     
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fShader_string, NULL); 
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
    if (status == GL_TRUE)
      std::cout << "Kompilacja shadera fragmentow powiodla sie!\n";
    else
    {
      std::cout << "Kompilacja shadera fragmentow NIE powiodla sie!\n";
      return 0;
    }

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    
    //glBindFragDataLocation(shaderProgram, 0, "outColor"); 

    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);
    return 1;
}



//--------------------------------------------funkcja inicjujaca-------------------------------------
int initGL(void)
{
   for(int i=0;i<26;i++)
   {
      ver_walec[3*i]=R*cos((i>>1)*M_PI/6);
      if(i%2==0)
         ver_walec[3*i+1]=0.5f;
      else
        ver_walec[3*i+1]=-0.5f;
      ver_walec[3*i+2]=R*sin((i>>1)*M_PI/6);


      /*if(i%2==0)
        col_walec[3*i]=0.2f;
      else
        col_walec[3*i]=0.5f;
      
      if(i%2==0)
        col_walec[3*i+1]=0.6f;
      else
        col_walec[3*i+1]=0.5f; 

      if(i%2==0)
        col_walec[3*i+2]=0.0f;
      else
        col_walec[3*i+2]=1.0f;*/
   }
    if(initShaders())
    {   
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL); 
        glGenVertexArrays(ilevao, vao); //przypisanie do vao identyfikatorow tablic
        glGenBuffers(ilevbo, vbo);	   //przypisanie do vbo identyfikatorow buforow
        glGenBuffers(ileebo, ebo);

        posAttrib = glGetAttribLocation(shaderProgram, "position"); //pobranie indeksu tablicy atrybutow wierzcholkow okreslajacych polozenie
              glEnableVertexAttribArray(posAttrib);
        colAttrib = glGetAttribLocation(shaderProgram, "color");    //pobranie indeksu tablicy atrybutow wierzcholkow okreslajacych kolor
              glEnableVertexAttribArray(colAttrib);
        
        glBindVertexArray(vao[0]);					//wybor tablicy
        glBindBuffer(GL_ARRAY_BUFFER, vbo[0]); 							//powiazanie bufora z odpowiednim obiektem (wybor bufora) 
        glBufferData(GL_ARRAY_BUFFER, sizeof(ver_triangle), ver_triangle, GL_STATIC_DRAW); 	//skopiowanie danych do pamieci aktywnego bufora
        glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);				//okreslenie organizacji danych w tablicy wierzcholkow
        glEnableVertexAttribArray(posAttrib);							//wlaczenie tablicy
        
        glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(col_triangle), col_triangle, GL_STATIC_DRAW);
        glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(colAttrib);
        
        glBindVertexArray(vao[1]);

        glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(ver_rectangle), ver_rectangle, GL_STATIC_DRAW);
        glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(posAttrib);

        glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(col_rectangle), col_rectangle, GL_STATIC_DRAW);
        glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(colAttrib);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[0]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

        glBindVertexArray(vao[2]); /*tworzenie prostopadloscianu*/

        glBindBuffer(GL_ARRAY_BUFFER, vbo[4]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(ver_prostopadloscian), ver_prostopadloscian, GL_STATIC_DRAW);
        glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(posAttrib);

        glBindBuffer(GL_ARRAY_BUFFER, vbo[5]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(col_prostopadloscian), col_prostopadloscian, GL_STATIC_DRAW);
        glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(colAttrib);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[1]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(prostopadloscianElements), prostopadloscianElements, GL_STATIC_DRAW);

        glBindVertexArray(vao[3]); /*tworzenie walca*/

        glBindBuffer(GL_ARRAY_BUFFER, vbo[6]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(ver_walec), ver_walec, GL_STATIC_DRAW);
        glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(posAttrib);

        glBindBuffer(GL_ARRAY_BUFFER, vbo[7]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(col_walec), col_walec, GL_STATIC_DRAW);
        glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(colAttrib);

       
      
        //macierz widoku (okresla polozenie kamery i kierunek w ktorym jest skierowana) 
        viewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f)); 
        //macierz rzutowania perspektywicznego
        projectionMatrix = glm::perspective(glm::radians(45.0f), 1.0f, 1.0f, 10.0f);		

        return 1;
          }
          else
        return 0;
}

//------------------------------------------renderowanie sceny-------------------------------------

int drawGLScene(int counter)
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    
    glm::mat4 translationMatrix = glm::translate(glm::mat4(), glm::vec3(0.0f, -0.4f, 0.0f));  		//macierz przesuniecia o zadany wektor
    glm::mat4 rotationMatrix =glm::rotate(glm::mat4(), glm::radians(fi), glm::vec3(0.0f, 1.0f, 0.0f)); //macierz obrotu o dany kat wokol wektora
		glm::mat4 scaleMatrix = glm::scale(glm::mat4(), glm::vec3(0.2, 0.4, 0.2));
    glm::mat4 transformMatrix = projectionMatrix * viewMatrix * translationMatrix *scaleMatrix; //wygenerowanie macierzy uwzgledniajacej wszystkie transformacje (zrob bez obrotu dla prostopadloscianu)


    GLint transformMatrixUniformLocation = glGetUniformLocation(shaderProgram, "transformMatrix");  //pobranie polozenia macierzy bedacej zmienna jednorodna shadera
    glUniformMatrix4fv(transformMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(transformMatrix)); //zapisanie macierzy bedacej zmienna jednorodna shadera wierzcholkow
    
    //glBindVertexArray(vao[0]);
    //glDrawArrays(GL_TRIANGLES, 0, 3); //rysowanie trojkata
    //glBindVertexArray(vao[1]);
    //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); //rysowanie prostokata
    glBindVertexArray(vao[3]);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 26); // Rysowanie walca
    //float startwys=-1.0f;
    for(int i=0;i<7;i++)
    {
      float wys=0.4f*i;
      float scale=pow(0.85f,i);
       translationMatrix = glm::translate(glm::mat4(), glm::vec3(0.0f,wys, 0.0f));  		
       glm::mat4 scaleMatrix = glm::scale(glm::mat4(), glm::vec3(scale,1.0f, scale));
      if(i%2==0)
         rotationMatrix = glm::rotate(glm::mat4(), glm::radians(-fi), glm::vec3(0.0f, 1.0f, 0.0f));
      else
         rotationMatrix = glm::rotate(glm::mat4(), glm::radians(fi), glm::vec3(0.0f, 1.0f, 0.0f));
       transformMatrix = projectionMatrix * viewMatrix * translationMatrix * rotationMatrix * scaleMatrix;
      GLint transformMatrixUniformLocation = glGetUniformLocation(shaderProgram, "transformMatrix");
      glUniformMatrix4fv(transformMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(transformMatrix));

      glBindVertexArray(vao[2]);
      glDrawElements(GL_TRIANGLES, 36 , GL_UNSIGNED_INT, 0); //rysowanie prostopadloscianu
    }
    
  
    fi += 0.5;
 
    glFlush();

    return 1;    
}

//----------------------------------------------------porzadki--------------------------------------

void deleteAll()
{
    glDeleteProgram(shaderProgram);
    glDeleteShader(fragmentShader);
    glDeleteShader(vertexShader);

    glDeleteBuffers(ilevbo, vbo);
    glDeleteBuffers(ileebo, ebo);
    glDeleteVertexArrays(ilevao, vao);
}
