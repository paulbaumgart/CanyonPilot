#ifndef __ObjReader_h__
#define __ObjReader_h__

	class ObjReader
	{
	public:
		static void  readObj(char* fileName, int &nVertices, float **vertices, float **normals, float **texcoords, int &nIndices, int **indices);

	private:
		static void get_indices(char *word, int *vindex, int *tindex, int *nindex);
	};

#endif 

