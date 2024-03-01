import json
import csv

info_list = []
def main():
    with open('Python\\file_operation\\data\\aip01.json', mode='r', encoding='utf-8') as f:
        info_list = json.load(f)
        
        # after loading the json data, we append two new titles.
        title = [['期数'] + list(info_list[0]) + ['收益率']]
        
        for k,item in enumerate(info_list, 1):
            data = [k] + list(item.values()) + [str(round( ( float(item['收益']) / float(item['投资总额']) ), 2)) + '%']
            title.append(data)
            
        info_list_new = title[:]
        
    with open('Python\\file_operation\\data\\aip01_new.csv', mode='w', encoding='utf-8') as f:
        writer = csv.writer(f)
        for r in info_list_new:
            writer.writerow(r)
        
if __name__ == '__main__':
    main()
        
        
        
    
    
