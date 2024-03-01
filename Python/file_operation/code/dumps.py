import json

# 要转换为 JSON 格式的数据
data = {"name": "John", "age": 30, "city": "New York"}

# 使用 json.dumps 将数据转换为 JSON 格式的字符串
json_string = json.dumps(data, indent=2)  # indent 参数用于增加输出的可读性
print(json_string)

# 将 JSON 格式的字符串写入文件
with open('Python\\file_operation\\data\\dumps_test_output.json', 'w') as file:
    file.write(json_string)