import csv
import json

# Path to your CSV file
csv_file_path = 'events.csv'

# Open and read the CSV file
with open(csv_file_path, newline='') as csvfile:
    reader = csv.reader(csvfile)
    for row in reader:
        # Each row is a list of strings. Assuming each row has a single JSON string.
        for json_str in row:
            try:
                # Parse the JSON string
                data = json.loads(json_str)
                
                # Check if data is a dictionary
                if isinstance(data, dict):
                    # Extract specific key values (e.g., "time" and "pod_usage")
                    time = data.get("time", "Key not found")
                    pod_usage = data.get("pod_usage", "Key not found")
                    
                    # Print or process the extracted values
                    print(f"Time: {time}, Pod Usage: {pod_usage}")
                else:
                    print(f"Expected a JSON object but got {type(data)}")
            except json.JSONDecodeError:
                print("Error decoding JSON")