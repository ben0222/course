import csv
import json

def main():
    
    info_list = []    
    with open('Python\\file_operation\\data\\aip01.csv', 'r', encoding='utf-8', newline='') as f:
        reader = csv.reader(f)
        for row in reader:
            info_list.append(row)
            
        
        
        # turn the inside list of info_list into a dictionary, so it can be dumped into a json file.
        for i in range(1, len(info_list)):
            # zip each row(info_list[i]) with the title row(info_list[0])
            info_list[i] = dict(zip(info_list[0], info_list[i]))
        
        # now info_list is a list composed with lots of dictionaries.
        
        with open('Python\\file_operation\\data\\aip01.json', mode='w', encoding='utf-8') as f:
            
            # while using dump(), do not forget to set the indent and ensure_ascii.
            json.dump(info_list[1:], f ,indent=4, ensure_ascii=False)
            
        print('You have successfully turned the aip01.csv into aip.01.json!')  
        

if __name__ == '__main__':
    main()
        