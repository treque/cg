///////////////////////////////////////////////////////////////////////////////
///  @file Modèle3D-OBJ.cpp
///  @author  Frédéric Plourde
///  @brief   Définit la classe CModele3DOBJ qui encapsule un modele3D
///           chargé à partir d'un fichier OBJ
///  @date    2008-10-27
///  @version 1.0
///////////////////////////////////////////////////////////////////////////////

#include "Modele3dObj.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////
///  @brief constructeur d'un modèle 3D avec 1 seule texture (utile seulement pour
///  faire des tests du côté openGL... pas utile pour le labo en tant que tel...
///
///  @param [in,out]  nomsFichierOBJ char *	    La chaîne de caractères contenant le nom de fichier OBJ du modèle 3D
///  @param [in,out]  nomFichierTexture char *     l'unique nom de fichier de l'unique texture
///  @param [in]       echelle float [=1.000000]    mise à l'échelle qui sera appliquée automatique (glScale)
///
///  @author Frédéric Plourde
///  @date   2008-10-27
///////////////////////////////////////////////////////////////////////////////
CModele3dObj::CModele3dObj(const string& nomFichierOBJ, const string& nomFichierTexture, float echelle)
    : CModele3dAbstrait(echelle, nomFichierTexture, TYPE_TEXTURE2D, true)
{
    // charger le modèle 3D en mémoire
    chargerModeleOBJ(nomFichierOBJ);
}

///////////////////////////////////////////////////////////////////////////////
///  @brief constructeur d'un modèle 3D avec plusieurs textures. Cette version du constructeur
///  est particulièrement utile lorsqu'on veut associer plusieurs textures aux différentes
///  unitées de texture du nuanceur.
///
///  @param [in,out]  nomsFichierOBJ        La chaîne de caractères contenant le nom de fichier OBJ du modèle 3D
///  @param [in,out]  nomsFichiersTextures  Le vecteur contenant la liste des noms de fichiers.
///  Cette liste est copiée, alors l'appelant peut en disposer.
///  @param [in]       echelle [=1.000000]    mise à l'échelle qui sera appliquée automatique (glScale)
///
///  @author Frédéric Plourde
///  @date   2008-10-27
///////////////////////////////////////////////////////////////////////////////
CModele3dObj::CModele3dObj(const string& nomFichierOBJ, const std::vector<std::string>& nomsFichiersTextures,
                           float echelle)
    : CModele3dAbstrait(echelle, nomsFichiersTextures, TYPE_TEXTURE2D, true)
{
    // charger le modèle 3D en mémoire
    chargerModeleOBJ(nomFichierOBJ);
}

///////////////////////////////////////////////////////////////////////////////
///  @brief Fonction qui charge en mémoire les sommets et les normales d'un modèle
///  définit dans un fichier .OBJ.
///
///  @return Aucune
///
///  @author Frédéric Plourde
///  @date   2008-10-27
///////////////////////////////////////////////////////////////////////////////
void CModele3dObj::chargerModeleOBJ(const std::string& fichierOBJ)
{
    CObjParser parser;

    if (parser.OpenFile(fichierOBJ))
    {
        cerr << "Lecture du fichier : " << fichierOBJ << endl;
        parser.ParseFile();
        parser.CloseFile();

        g_Vertices_    = parser.GetVertices();
        g_Triangles_   = parser.GetTriangles();
        g_Quads_       = parser.GetQuads();
        g_ObjCentroid_ = parser.GetCentroid();

        cerr << "Centroid: " << g_ObjCentroid_ << endl;
        cerr << "Nombre de faces : " << (g_Triangles_.size() + g_Quads_.size()) << endl;

        vector<float> vertex_vector = getFloatsFromVertices(g_Vertices_);
        int           byte_size     = vertex_vector.size() * sizeof(float);

        // Notre Vertex Array Object - VAO
        glGenVertexArrays(1, &triangles_vao_);
        // Un buffer pour les sommets
        glGenBuffers(1, &sommets_vbo_);
        // Un buffer pour les indices des sommets des triangles
        glGenBuffers(1, &triangles_ibo_);

        // Lier
        glBindVertexArray(triangles_vao_);
        glBindBuffer(GL_ARRAY_BUFFER, sommets_vbo_);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangles_ibo_);

        // Peupler
        glBufferData(GL_ARRAY_BUFFER, byte_size, &vertex_vector[0], GL_STATIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, g_Triangles_.size() * sizeof(Triangle), &g_Triangles_[0].Vertex[0],
                     GL_STATIC_DRAW);

        glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size_);

        // glBindBuffer(GL_ARRAY_BUFFER, m_sommets_vbo);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr); // texcoords
        glEnableVertexAttribArray(0);

        // Normals
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (const void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (const void*)(5 * sizeof(float)));
        glEnableVertexAttribArray(2);

        cerr << "Chargement termine !!" << endl;
    }
}

///////////////////////////////////////////////////////////////////////////////
///
///
///  @return Aucune
///
///  @author
///  @date
///
///////////////////////////////////////////////////////////////////////////////
void CModele3dObj::appliquerTextures()
{
}

/// fonction qui permet de definir un nuanceur pour l'objet.
// void CModele3DOBJ::attribuerNuanceur(CNuanceurProg nuanceur)
//{
//	m_nuanceur_prog = nuanceur;
//}

///////////////////////////////////////////////////////////////////////////////
///  @brief Fonction qui affiche le modèle 3D à partir des tableaux de sommets lus dans le fichier source OBJ.
///
///  @return Aucune
///
///  @author Félix G. Harvey
///  @date   2016
///////////////////////////////////////////////////////////////////////////////
void CModele3dObj::dessiner()
{
    if (!g_Triangles_.empty())
    {
        glDrawElements(GL_TRIANGLES,    // mode
                       size_,           // count
                       GL_UNSIGNED_INT, // type
                       nullptr          // element array buffer offset
        );
    }
}
