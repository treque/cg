///////////////////////////////////////////////////////////////////////////////
///  @file Main.cpp
///
///  @brief    Cette application implémente un engin simple de lancer de rayons a
///            des fins éducationnelles du cours "Infographie avancée - INF8702".
///            L'implémentation et le design original du lancer de rayons de Polytechnique
///            fut écrite par Yves Martel (2003).
///            Cette version est plus modulaire et est plus facile à maintenir et à comprendre
///            Elle fut également conçue de telle façon à pouvoir être portée facilement sur GPU
///            en GLSL.
///  @remarks  Les vieux fichiers de donnnées (.dat) ne fonctionne plus avec cette version.
///            Vous devrez utiliser les fichiers .dat fournis avec le présent projet jusqu'à ce
///            qu'un meilleur format de fichier soit implémenté. Un format simple comme OBJ est
///            présentement considéré à cet égard.
///
///  @author  Olivier Dionne
///  @date    13/08/2008
///  @version 1.1.0
///
///////////////////////////////////////////////////////////////////////////////
//#if _WIN32
//#include <windows.h>
//#else
//#include <sys/time.h>
//#endif

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Cst.h"
#include "HighResolutionTimer.h"
#include "NuanceurProg.h"
#include "Var.h"

using namespace std;
using namespace Scene;

static GLuint quad_vbo;
static GLuint quad_ibo;
static GLuint quad_vao;
static GLint  quad_size;

// les programmes de nuanceurs
static CNuanceurProg progNuanceurQuad("Nuanceurs/quadSommets.glsl", "Nuanceurs/quadFragments.glsl", false);

// Déclarations des méthodes
void Initialiser(void);
void Redimensionner(GLFWwindow* fenetre, int w, int h);
void Liberer(void);
void Dessiner(void);
void initialiserQuad(void);

int main(int argc, char* argv[])
{
    GLFWwindow* fenetre;
    // Traiter les arguments d'entrée
    if (argc > 1)
    {
        int XRes = CVar::g_LargeurFenetre;
        int YRes = CVar::g_HauteurFenetre;

        if (argc > 2)
        {
            for (int i = 2; i < argc; i++)
            {
                if (*argv[i] == '-')
                {
                    switch (argv[i][1])
                    {
                    case 'x': XRes = atoi(argv[++i]); break;
                    case 'y': YRes = atoi(argv[++i]); break;
                    }
                }
            }
        }

        // S'assurer que la résolution fournie est une puissance de deux.
        if (((XRes - 1) & XRes) || ((YRes - 1) & YRes))
        {
            cerr << "[ERREUR]: Resolution " << XRes << "x" << YRes << " n'est pas une puissance de deux" << endl;
            system("pause");
            exit(1);
        }

        // Construire la scène
        CVar::g_GestionnaireDeScene = CScene::ObtenirInstance();

        // Ajuster la résolution de la scène
        CVar::g_LargeurFenetre = XRes;
        CVar::g_HauteurFenetre = YRes;
        CVar::g_GestionnaireDeScene->AjusterResolution(CVar::g_LargeurFenetre, CVar::g_HauteurFenetre);

        // Traiter le fichier de données de la scène
        cout << "[ETAT]: Traitement du fichier de donnees de la scene..." << endl;
        CVar::g_GestionnaireDeScene->TraiterFichierDeScene(argv[1]);

        cout << "[ETAT]: Initialisation de glfw..." << endl;

        // start GL context and O/S window using the GLFW helper library
        if (!glfwInit())
        {
            fprintf(stderr, "ERREUR: impossible d'initialiser GLFW3\n");
            return 1;
        }

        fenetre = glfwCreateWindow(CVar::g_LargeurFenetre, CVar::g_HauteurFenetre, "INF8702 - Labo", nullptr, nullptr);
        if (!fenetre)
        {
            fprintf(stderr, "ERREUR: impossibe d'initialiser la fenêtre avec GLFW3\n");
            glfwTerminate();
            return 1;
        }
        glfwSetWindowPos(fenetre, 600, 100);

        // Rendre le contexte openGL courrant celui de la fenêtre
        glfwMakeContextCurrent(fenetre);

        // Combien d'updates d'écran on attend après l'appel à glfwSwapBuffers()
        // pour effectivement échanger les buffers et retourner
        glfwSwapInterval(1);

        // Définir la fonction de redimensionnement
        glfwSetWindowSizeCallback(fenetre, Redimensionner);

        // vérification de la version 4.X d'openGL
        glewInit();
        if (glewIsSupported("GL_VERSION_4_5"))
            printf("Pret pour OpenGL 4.5\n\n");
        else
        {
            printf("\nOpenGL 4.5 n'est pas supporte! \n");
            exit(1);
        }

        // Specifier le context openGL
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    }
    else
    {
        cerr << "[ERREUR]: Aucune fichier de scene en argument !" << endl;
        system("pause");
        exit(1);
    }

    // Initialiser OpenGL
    Initialiser();

    // Boucle principale
    while (!glfwWindowShouldClose(fenetre))
    {
        glfwPollEvents();

        // Afficher nos modèlests
        Dessiner();

        // put the stuff we've been drawing onto the display
        glfwSwapBuffers(fenetre);
    }
    // close GL context and any other GLFW resources
    glfwTerminate();

    // Libérer les données de l'application
    Liberer();

    return EXIT_SUCCESS;
}

void Initialiser(void)
{
    // Activer le test de profondeur d'OpenGL
    glEnable(GL_DEPTH_TEST);

    // Définir la fonction du test de profondeur
    glDepthFunc(GL_LEQUAL);

    // Compiler et lier le nuanceur
    progNuanceurQuad.compilerEtLier();

    // Initialiser les vbo, ibo, vao nécessaires pour un quad.
    initialiserQuad();

    cout << "[ETAT]: Lancer de rayons..." << endl;

    HighResolutionTimer renderTime;
    renderTime.start();

    // Générer la scène par lancers de rayons
    CVar::g_GestionnaireDeScene->LancerRayons();

    renderTime.stop();

    cout << "[ETAT]: Termine! --> Temps total de rendu : " << renderTime.getAvgTimeMs() / 1000.f << " secondes" << endl;
}

void Redimensionner(GLFWwindow* /* fenetre */, int w, int h)
{
    CVar::g_LargeurViewport = w;
    CVar::g_HauteurViewport = h;
    glViewport(0, 0, w, h);
}

void Liberer(void)
{
    CVar::g_GestionnaireDeScene->LibererInstance();
    CVar::g_GestionnaireDeScene = nullptr;
}

void initialiserQuad(void)
{
    std::vector<float>        sommets = {-1, -1, 0, 1, -1, 0, 1, 1, 0, -1, 1, 0};
    std::vector<unsigned int> indices = {0, 1, 2, 2, 3, 0};

    // Notre Vertex Array Object - VAO
    glGenVertexArrays(1, &quad_vao);
    // Un buffer pour les sommets
    glGenBuffers(1, &quad_vbo);
    // Un buffer pour les indices
    glGenBuffers(1, &quad_ibo);

    glBindVertexArray(quad_vao);
    glBindBuffer(GL_ARRAY_BUFFER, quad_vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quad_ibo);
    auto s_byte_size = static_cast<int>(sommets.size() * sizeof(float));
    auto i_byte_size = static_cast<int>(indices.size() * sizeof(unsigned int));
    glBufferData(GL_ARRAY_BUFFER, s_byte_size, &sommets[0], GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, i_byte_size, &indices[0], GL_STATIC_DRAW);

    quad_size = static_cast<int>(indices.size());

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr); // texcoords
    glEnableVertexAttribArray(0);
}

void Dessiner(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    progNuanceurQuad.activer();

    glEnable(GL_TEXTURE_2D);

    // Lier la texture de scène au quad OpenGL
    glBindTexture(GL_TEXTURE_2D, CVar::g_GestionnaireDeScene->ObtenirTextureGL());

    glBindVertexArray(quad_vao);
    glDrawElements(GL_TRIANGLES,    // mode
                   quad_size,       // count
                   GL_UNSIGNED_INT, // type
                   nullptr          // element array buffer offset
    );

    glDisable(GL_TEXTURE_2D);
}
