#include <iostream>
#include <iomanip>
#include <string>

#define LD 0

using namespace std;

struct attributes {

	int instr;
	string dest, src;

	bool post, pre;
	bool inc, dec;
	bool left, right;
};

bool bin_post_inc(struct attributes *attr, string line) {

	int pos, inc_pos;
	line = line.substr(2, line.size());

	if((pos = line.find(",")) == string::npos)
		return false;

	string dest = line.substr(0, pos);
	string src = line.substr(pos + 1, line.size());

	if((inc_pos = dest.find("+")) != string::npos) {

		if(inc_pos != dest.size() - 1) {

			if(inc_pos != 0)
				return false;

			attr->pre = true;
			dest = dest.substr(1, dest.size());
			
		} else {

			attr->post = true;
			dest = dest.substr(0, inc_pos);
		}
		
		attr->left = true;
		goto end;		
	}

	if((inc_pos = src.find("+")) != string::npos) {

		if(inc_pos != src.size() - 1) {

			if(inc_pos != 0)
				return false;

			attr->pre = true;
			src = src.substr(1, src.size());

		} else {

			attr->post = true;
			src = src.substr(0, inc_pos);
		}

		attr->right = true;
		goto end;
	}

end:

	/* Validate Destination and Source */

	attr->dest = dest;
	attr->src = src;

	return true;
}

void is_ld(struct attributes *attr, string line) {

	if(bin_post_inc(attr, line) == false)
		exit(-1);

	/* Validate other variants of binary Ops */	
}

int main(void) {

	struct attributes attr;
	string post_i = "ldr0,+X";
	
	if(bin_post_inc(&attr, post_i) == false) {
	
		cout << "False" << endl;
		return EXIT_FAILURE;
	}

	cout << "Line: " << post_i << endl << endl;

	cout << "Dest: " << attr.dest << endl;
	cout << "Src: " << attr.src << endl;
	cout << "Left: " << attr.left << endl;
	cout << "Right: " << attr.right << endl;
	cout << "Increment: " << attr.inc << endl;
	cout << "Decrement: " << attr.dec << endl;
	cout << "Post: " << attr.post << endl;
	cout << "Pre: " << attr.pre << endl;

	return EXIT_SUCCESS;
}