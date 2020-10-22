#include "ObjParser.h"
#include "Vecteur3.h"

using namespace Math3D;
using namespace std;

COBJParser::COBJParser(void)
    : CParserTextAbstrait()
    , m_Vertices(0)
    , m_Triangles(0)
    , m_Quads(0)
    , m_Centroid(CVecteur3::ZERO)
{
}

COBJParser::~COBJParser(void)
{
    m_Vertices.clear();
    m_Triangles.clear();
    m_Quads.clear();
}

void COBJParser::ParseFile(void)
{
    if (m_IsOpen)
    {
        const uint32_t MAX_NUM_CHAR = 100;
        vector<char>   Temp(MAX_NUM_CHAR);
        string         Buffer;

        vector<CVecteur3> VertCoords;
        vector<CVecteur3> Normals;
        vector<float>     TexCoords;

        CVecteur3 MaxPt, MinPt;

        float x, y, z, u, v;

        while (!m_FileStream.eof())
        {
            m_FileStream.getline(Temp.data(), MAX_NUM_CHAR);
            Buffer = Temp.data();
            if (Buffer.empty())
                continue;

            // Skip comments
            if (Buffer[0] == '#')
                continue;
            else if (Buffer[0] == 'v')
            {
                // Normal
                if (Buffer[1] == 'n')
                {
                    sscanf(Buffer.c_str(), "%s %f %f %f", Temp.data(), &x, &y, &z);
                    Normals.push_back(CVecteur3(x, y, z));
                }
                // Texture coordinate
                else if (Buffer[1] == 't')
                {
                    sscanf(Buffer.c_str(), "%s %f %f", Temp.data(), &u, &v);
                    TexCoords.push_back(u);
                    TexCoords.push_back(v);
                }
                // Vertex
                else
                {
                    sscanf(Buffer.c_str(), "%s %f %f %f", Temp.data(), &x, &y, &z);
                    CVecteur3 Vert(x, y, z);

                    // Get OBJ's AABB
                    if (Vert.x > MaxPt.x)
                        MaxPt.x = Vert.x;
                    if (Vert.y > MaxPt.y)
                        MaxPt.y = Vert.y;
                    if (Vert.z > MaxPt.z)
                        MaxPt.z = Vert.z;
                    if (Vert.x < MinPt.x)
                        MinPt.x = Vert.x;
                    if (Vert.y < MinPt.y)
                        MinPt.y = Vert.y;
                    if (Vert.z < MinPt.z)
                        MinPt.z = Vert.z;

                    VertCoords.push_back(Vert);
                }
            }
            // Face
            else if (Buffer[0] == 'f')
            {
                vector<string> Tokens = StringUtils::Tokenize(Buffer, " ");
                vector<string> SplitToken;

                // Create vertices
                for (unsigned int i = 1; i < Tokens.size(); i++)
                {
                    SplitToken = StringUtils::Tokenize(Tokens[i], "/");

                    // Set vertex position and normal
                    Vertex Vert(
                        VertCoords[static_cast<uint32_t>(atoi(SplitToken[0].c_str())) - 1],
                        Normals[static_cast<uint32_t>(atoi(SplitToken[TexCoords.empty() ? 1 : 2].c_str())) - 1]);

                    // Set vertex texture coords
                    if (!TexCoords.empty())
                    {
                        uint32_t TexIndex = static_cast<uint32_t>((atoi(SplitToken[1].c_str())) - 1) << 1;
                        Vert.Texture[0]   = TexCoords[TexIndex + 0];
                        Vert.Texture[1]   = TexCoords[TexIndex + 1];
                    }

                    m_Vertices.push_back(Vert);
                }

                // Create faces
                unsigned int Count = static_cast<unsigned int>(m_Vertices.size());
                if (Tokens.size() > 4)
                    m_Quads.push_back(Quad(Count - 4, Count - 3, Count - 2, Count - 1));
                else
                    m_Triangles.push_back(Triangle(Count - 3, Count - 2, Count - 1));
            }
        }

        // Calculate centroid
        m_Centroid = (MaxPt + MinPt) * 0.5f;
    }
}
