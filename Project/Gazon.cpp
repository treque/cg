///////////////////////////////////////////////////////////////////////////////
///  @file Gazon.cpp
///  @author  Frédéric Plourde
///  @brief   Définit la classe CGazon qui encapsule une patch bsplinaire antialiasée
///  @date    2007-12-11
///  @version 1.0
///
///////////////////////////////////////////////////////////////////////////////

#include "Gazon.h"

///////////////////////////////////////////////////////////////////////////////
///  public constructor  CGazon \n
///  Construit un skybox au moyen d'une seule texture (seule possibilité pour un skybox)
///
///  @return Aucune
///
///  @author Frédéric Plourde
///  @date   2008-02-20
///
///////////////////////////////////////////////////////////////////////////////
CSea::CSea(const char* nomFichierTexture, const float /* largeurGazon */, const float /* longueurGazon */)
    : CModele3DAbstrait(1.0, nomFichierTexture, TYPE_TEXTURE2D, true)
{
}

///////////////////////////////////////////////////////////////////////////////
///  public destructor  ~CGazon \n
///  Destructeur par défaut d'un skybox
///
///  @return Aucune
///
///  @author Frédéric Plourde
///  @date   2008-02-20
///
///////////////////////////////////////////////////////////////////////////////
CSea::~CSea(void)
{
}

///////////////////////////////////////////////////////////////////////////////
///  public static  dessiner \n
///  Fonction à implémenter absolument si on veut voir apparaître quelque chose !
///  cette fonction sera appelée par la classe de base (CModele3DAbstrait) par
///  l'entremise d'une pointeur de fonction passé à la construction. On mettra ici
///  uniquement le graphisme associé à l'objet (i.e.  glBegin()...glVertex(), etc...)
///
///  @return Aucune
///
///  @author Frédéric Plourde
///  @date   2008-02-20
///
///////////////////////////////////////////////////////////////////////////////
void CSea::dessiner()
{
    //appliquerTextures();
    glBindVertexArray( m_vao );
    glEnableVertexAttribArray( 0 );
    glEnableVertexAttribArray( 1 );

    glDrawElements( GL_TRIANGLES, m_taille, GL_UNSIGNED_INT, NULL );

    glDisableVertexAttribArray( 0 );
    glDisableVertexAttribArray( 1 );
}

void CSea::appliquerTextures()
{
    // texturer le gazon
    textures_[0]->appliquer(WRAP_S | WRAP_T | MIN_FILTER | MAG_FILTER, GL_REPEAT, GL_REPEAT, 0,
                            CCst::mapFilters[CVar::minFilter], CCst::mapFilters[CVar::magFilter], 0, 0, 0, 0, 0, 0,
                            true, GL_TEXTURE0);
}

void CSea::setupVAO()
{
    // Sommets du cube:
    float sommets[] = {
        // Positions
        1.0f, 1.0f, 0.0f,

        1.0f, -1.0f, 0.0f,

        -1.0f, -1.0f, 0.0f,

        -1.0f, 1.0f, 0.0f,

    };

    // Indique les indexes des sommets composant les faces du cube (groupés en triangles)
    unsigned int indices_sommets[] = { 0,  1,  3,
                                       1, 2, 3};

    // Notre Vertex Array Object - VAO
    glGenVertexArrays( 1, &m_vao );
    // Un buffer pour les sommets
    glGenBuffers( 1, &m_vbo );
    // Un buffer pour les indices des sommets
    glGenBuffers( 1, &m_ibo );

    glBindVertexArray( m_vao );
    glBindBuffer( GL_ARRAY_BUFFER, m_vbo );
    glBufferData( GL_ARRAY_BUFFER, sizeof( sommets ), sommets, GL_STATIC_DRAW );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // Retenir la taille nécessaire a dessiner plus tard.
    m_taille = sizeof( sommets );


    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_ibo );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( indices_sommets ), &indices_sommets[ 0 ], GL_STATIC_DRAW );

    glBindVertexArray( 0 );
    // glBindBuffer( GL_ARRAY_BUFFER, m_vbo );
    //glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof( float ), nullptr );
    // glEnableVertexAttribArray(0);
    //glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof( float ), (char*)( 16 ) );
    // glEnableVertexAttribArray(1);
}
