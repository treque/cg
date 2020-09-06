#ifndef OBJ_PARSER
#define OBJ_PARSER

#include <iostream>
#include <vector>

#include "Geometry.h"
#include "ParserTextAbstrait.h"
#include "StringUtils.h"

class CObjParser : public CParserTextAbstrait
{
public:
    CObjParser(void);
    virtual ~CObjParser(void);

    void ParseFile(void);

    inline const std::vector<Vertex>   GetVertices(void) const;
    inline const std::vector<Triangle> GetTriangles(void) const;
    inline const std::vector<Quad>     GetQuads(void) const;
    inline const CVecteur3             GetCentroid(void) const;

private:
    std::vector<Vertex>   m_Vertices;
    std::vector<Triangle> m_Triangles;
    std::vector<Quad>     m_Quads;

    CVecteur3 m_Centroid;
};

const std::vector<Vertex> CObjParser::GetVertices(void) const
{
    return m_Vertices;
}

const std::vector<Triangle> CObjParser::GetTriangles(void) const
{
    return m_Triangles;
}

const std::vector<Quad> CObjParser::GetQuads(void) const
{
    return m_Quads;
}

const CVecteur3 CObjParser::GetCentroid(void) const
{
    return m_Centroid;
}

#endif // OBJ_PARSER
