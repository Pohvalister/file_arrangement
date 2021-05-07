#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <stack>
#include <string>
#include <sstream>

using namespace std;


struct FileNode {
	string name;
	vector<FileNode*> dependencies;
};

vector<FileNode*> nodes_list;

unordered_map<string, vector<FileNode*>> depender_map;
unordered_map<string, FileNode*> definer_map;


vector<string> coloring() {
	unordered_set<FileNode*> fulfilment_set;

	vector<string> answ;

	for (auto node : nodes_list) {
		if (fulfilment_set.find(node) != fulfilment_set.end() ) {
			continue;
		}

		stack<FileNode*> touched_nodes;

		touched_nodes.push(node);

		while (!touched_nodes.empty()) {
			FileNode* current_node = touched_nodes.top();

			for (auto granter : current_node->dependencies) 
				if (fulfilment_set.find(granter) == fulfilment_set.end()) {
					touched_nodes.push(granter);
					continue;
				}

			fulfilment_set.insert(current_node);
			answ.push_back(current_node->name);
			touched_nodes.pop();
		}
		
	}
	
	reverse(answ.begin(), answ.end());
	return answ;
}


int main(int argc, char* argv[]){
	//if (argc != 2) {
	//	std::cerr << "usage:" << argv[0] << " input_file output_file" << std::endl;
	//	return 1;
	//}
	ifstream in("test.txt");
	

 	string file_data;
	while (getline(in, file_data)) {
		string delimiter = ";";
		stringstream define_s(file_data.substr(0, file_data.find(delimiter)));
		stringstream depend_s(file_data.substr(file_data.find(delimiter) + delimiter.length(), file_data.size()));
		

		string lib_name;
		define_s >> lib_name;

		FileNode* tmp = new FileNode();
		tmp->name = lib_name;

		nodes_list.push_back(tmp);

		string func_name;
		while (depend_s >> func_name) {
			if (depender_map.find(func_name) == depender_map.end())
				depender_map[func_name] = vector<FileNode*>();

			depender_map[func_name].push_back(tmp);
		}

		while (define_s >> func_name) 
			definer_map[func_name] = tmp;
		
	}

	for (pair<string, vector<FileNode*>> map_data : depender_map) {
		for (FileNode* node : map_data.second) {
			node->dependencies.push_back(definer_map[map_data.first]);
		}
	}

	vector<string> output = coloring();

	for (string str : output) {
		cout << str << ' ';
	}

	return 0;
}
