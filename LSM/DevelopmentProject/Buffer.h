#ifndef BUFFER_H
#define BUFFER_H

#include <cmath>

namespace Data{

	class Buffer{

	private:
		int maxSize = 4096;
		char *buf;
		int size;
		int currIdx = 0;

	public:
		Buffer(int size){
			int s = std::floor(maxSize / size);
			size = s * size;
			buf = new char[size];
			this->size = size;
		};

		~Buffer(){
			delete buf;
		}; 

		/*
		* Write buffer only
		*/
		bool write(char* &data, int length){

			if (length > (this->size - currIdx)){
				return false;
			}

			//copy to buffer
			for (int i = 0; i < length; i++){
				this->buf[currIdx + i] = data[i];
			}

			currIdx = currIdx + length;
			return true;
		}

		int &getCurrIdx(){
			return this->currIdx;
		};

		void reset(){
			this->currIdx = 0;
		};

		/*
		* Read Buffer only
		*/ 
		char* &getBuffer(){
			return this->buf;
		};

	}; 
};
#endif 