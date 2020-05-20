#include <iostream>
#include <stdio.h>
#include <locale.h>
#include <fstream>
#include <unordered_map> 
#include<string>
#include<map>
#include<set>
#include <functional>//для хеша
#include <boost/filesystem.hpp>
using namespace boost::filesystem;
using namespace std; 

int hash_func(string path){
	FILE* fl = NULL;
    fl = fopen(path.c_str(), "r");
	fseek(fl, 0 , SEEK_END);                        
    long lSize = ftell(fl);                            
    rewind (fl);
	char* hs;
    hs = (char*)malloc(lSize);
	long result = fread(hs, 1, lSize, fl);
	fclose(fl);
	string hash_re = (char*)hs;
	std::hash<std::string> hash_fn;
    int str_hash = hash_fn(hash_re);
	return str_hash;
}

int files_eq(string s1, string s2){
	FILE* first_file = fopen(s1.c_str(), "r");
	FILE* second_file = fopen(s2.c_str(), "r");
	int f = 1;
	char c1, c2;
	do{
		c1 = getc(first_file);
		c2 = getc(second_file);
		if(c1 != c2){
			fclose(first_file);
			fclose(second_file);
			return 0;	
		}
	}while(c1!=EOF);
	fclose(first_file);
	fclose(second_file);
	return 1;
}

int main(int argc, char *argv[])
{
	path cur_program{argv[0]};
	set<string>cur_file;//сет для хранения кандидатов на удаление
	set<int>cur_files_n;//сет для хранения номеров для сохранения
	map<int, set<string>>ma;

	/*todo  
		ma[file_size(p.path())].insert(canonical(p.path()).c_str());
		бежим по сету и делаем кеш функцию. Если кеш совпал, то полностью сравним файлы
		Кеш посчитаем в мапе.
		если совпал, то пробежимся по созданной мапе и выполним часть кода с удалением
		как только удалим файл, далаем ma[file_size(p.path())].erase(параметр, из auto)
	*/

	map<string, int>mp;
	/*формируем ассоциативный массив с ключом на размер и значением сет путей*/
	path dir{argv[1]};
    for (auto & p:recursive_directory_iterator(dir)){
		if(is_regular_file(p.symlink_status())){
			ma[file_size(p.path())].insert(canonical(p.path()).c_str());
		} 		 
	}

	if(argc == 3){
		if(strcmp(argv[2], "see_all") == 0){
			for(auto [sz,se]: ma){
				for(auto path1 :se)
					cout << path1 << " " << sz<< "B" << endl;
			}
		}
		cout << endl;
	}
	map<int, set<string>>hash_1;

	/*пробежим по всем размерам*/

	for(auto [sz, se]: ma) {
		if(se.size() == 1)
			continue; // проверка, что несколько файлов одного размера
		hash_1.clear();
		/*чтобы не считать хеш для абсолютно всех файлов, создадим отдельные сеты для файлов уже с одинаковым размером*/	
	
		for(auto path1: se){	
			hash_1[hash_func(path1)].insert(path1);
		}
	
		for(auto [hash_ind, set_of_paths]: hash_1){ //пробежим по всем хешам
		if(set_of_paths.size() == 1) //проверка, что файлов несколько
			continue;

		for(auto path1: set_of_paths){
			for(auto path4: set_of_paths){
				if(files_eq(path1, path4)){
					cur_file.insert(path1);
					mp[path1] = 1;
				}
			}
		}
		
		/* если нашли хотя бы два одинаковых*/
		if(cur_file.size() != 0){ 
			int counter = 1;
			for(auto to: cur_file){
				cout <<counter << ')'<< to << endl;
				counter++;
			}
			cout << "Do you want to delete some of them? Please, type Y or N" << endl;
			char ans;
			cin >> ans;
			while(ans != 'N' && ans != 'Y'){
				cout << "Type again, please\n";
				cin >> ans;
			}
			if(ans == 'N'){
				cout << "Closed\n";
				cur_file.clear();
				
				cur_files_n.clear();
				for(auto [path3, se3]: mp)
					ma[sz].erase(path3);
				mp.clear();
				continue;
				
			}
			cout << "How many files do you want to save" << endl;
			int cou;
			cin >> cou;
			cout << "Which files do you want to save?" << endl;
			int o;
			for(int i = 0; i < cou; i++){
				cin >> o;
				cur_files_n.insert(o);
			}
			int cur = 1;			
			for(auto [path3, se3]: mp){
				ma[sz].erase(path3);
				if(cur_files_n.count(cur) != 1){
						if(path3 == canonical(cur_program).c_str()){    // не пытаемся ли мы удалить исполняемый файл
							cout << "I can not delete this file" <<path3<<endl; 			
						}else{
							ma[sz].erase(path3);
							path rm{path3};
							if(remove(canonical(rm).c_str()) == -1)
								cout << "error";
						}
				}
				cur++;
				
			} 
		}
		cur_files_n.clear();//номера файлов, которые не хотим удалять
		mp.clear();//кандидаты на удаление
		cur_file.clear();//пути кандидатов на удаление
	}
		
	}
    //remove_all("sandbox");
}