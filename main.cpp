#include <iostream>
#include <fstream>
#include <sstream>

#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <string>
#include <stack>


struct FileNode {

	FileNode(std::string n)
		: name(n)
	{}

	std::string name;
	std::vector<FileNode*> dependencies;
};

static std::vector<FileNode*> nodes_list;

static std::unordered_map<std::string, std::vector<FileNode*>> depender_map;
static std::unordered_map<std::string, FileNode*> definer_map;


std::vector<std::string> SortFileDependencies() {
	std::unordered_set<FileNode*> fulfilment_set;

	std::vector<std::string> sorted_names;

	for (auto node : nodes_list) {
		//Taking only nodes that wasn't considered
		if (fulfilment_set.find(node) != fulfilment_set.end()) {
			continue;
		}

		std::stack<FileNode*> touched_nodes;

		touched_nodes.push(node);

		while (!touched_nodes.empty()) {
			FileNode* current_node = touched_nodes.top();

			//Adding source nodes before considering current
			for (auto source_node : current_node->dependencies) 
				if (fulfilment_set.find(source_node) == fulfilment_set.end()) {
					touched_nodes.push(source_node);
					continue;
				}

			fulfilment_set.insert(current_node);
			sorted_names.push_back(current_node->name);
			touched_nodes.pop();
		}
	}
	
	return sorted_names;
}


int main(int argc, char* argv[]){

	/*if (argc != 3) {
		std::cerr << "Incorrect argument, usage:" << argv[0] << " INPUT_FILE OUTPUT_FILE" << std::endl;
		return -1;
	}*/


	try {
		//Reading file data
		std::ifstream input_file("test_input.txt");//argv[1]);


		std::string file_data;
		while (getline(input_file, file_data)) {

			std::string delimiter = ";";
			std::size_t split_place = file_data.find(delimiter);
			std::stringstream define_stream(file_data.substr(0, split_place));
			std::stringstream depend_stream(file_data.substr((split_place != -1 ? split_place + delimiter.length() : file_data.size()), file_data.size()));

			std::string lib_name;
			define_stream >> lib_name;

			FileNode* lib_node = new FileNode(lib_name);
			nodes_list.push_back(lib_node);

			std::string func_name;

			while (define_stream >> func_name)
				definer_map[func_name] = lib_node;//because of One Definition Rule

			while (depend_stream >> func_name) {
				if (depender_map.find(func_name) == depender_map.end())
					depender_map[func_name] = std::vector<FileNode*>();

				depender_map[func_name].push_back(lib_node);
			}

		}

		for (const std::pair<std::string, std::vector<FileNode*>> &map_data : depender_map) {
			for (FileNode* node : map_data.second)
				node->dependencies.push_back(definer_map[map_data.first]);
		}
			

		std::vector<std::string> sorted_names = SortFileDependencies();

		reverse(sorted_names.begin(), sorted_names.end());

		std::ofstream output_file("test_output.txt");

		for (const std::string& name  : sorted_names) {
			output_file << name << ' ';
			//std::cout << str << ' ';
		}
	}
	catch (const std::exception& e) {
		std::cerr << "ERROR:: " << e.what() << std::endl;
		return -1;
	}

	return 0;
}
