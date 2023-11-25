use serde::Deserialize;
use std::collections::HashMap;
use std::fs::File;
use std::io::Read;
use rayon::prelude::*;
use std::io::{Write};
use std::path::Path;
use serde_json::Value;


#[derive(Debug, Deserialize)]
struct JsonData {
    // Assuming your JSON file has a list of key-value pairs.
    // Adjust the structure accordingly based on your JSON file.
    key: String,
    value: i32,
}

pub fn read_json_file(file_path: &str) -> Result<HashMap<String, i32>, String> {
    // Read the JSON file
    let mut file = File::open(file_path).map_err(|e| format!("Failed to open file: {}", e))?;
    let mut json_string = String::new();
    file.read_to_string(&mut json_string).map_err(|e| format!("Failed to read file: {}", e))?;
    println!("{}", json_string);
    // json_string = json_string.replace("\n", "").replace(" ", "").replace("\t", "");

    // Deserialize JSON into a Vec<JsonData>
    // println!("{:?}", serde_json::from_str(&json_string));
    // let data: Result<Vec<JsonData>, serde_json::Error> = serde_json::from_str(&json_string);
    // println!("{:?}", data);

    // Create a HashMap<String, i32> from the deserialized data
    // match data {
    //     Ok(json_data) => {
    //         let result_map: HashMap<String, i32> = json_data
    //             .into_iter()
    //             .map(|item| (item.key, item.value))
    //             .collect();

    //         Ok(result_map)
    //     }
    //     Err(e) => Err(format!("Error parsing JSON: {}", e)),
    // }
    
    let json_value: Value = serde_json::from_str(&json_string).unwrap();
    println!("{:?}", json_value);

    let hashmap: HashMap<String, i32> = json_value
        .as_object()
        .expect("Expected a JSON object")
        .iter()
        .map(|(key, value)| {
            let int_value = value.as_i64().expect("Expected an integer") as i32;
            (key.clone(), int_value)
        })
        .collect();

    Ok(hashmap)
}

pub fn args_to_str(args: &HashMap<String, i32>) -> String {
    let result: String = args
        .par_iter()
        .map(|(key, value)| format!("_{}_{}", key, value))
        .collect();

    result
}

pub fn write_csv(filename: &str, data: &Vec<Vec<f64>>) {
    let path = Path::new(filename);
    let mut file = match File::create(&path) {
        Ok(file) => file,
        Err(err) => {
            eprintln!("Error opening the file: {} ({})", filename, err);
            return;
        }
    };

    for row in data {
        for cell in row {
            write!(file, "{},", cell).expect("Error writing to file");
        }
        // Move to the next line after each row
        writeln!(file).expect("Error writing to file");
    }

    println!("Data written to {} successfully.", filename);
}