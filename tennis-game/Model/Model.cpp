class Model {

SourceData sourceData;
public:

Model( SourceData sourceData ) : sourceData( sourceData ) {}

void selectObject(DataConfig data_config, std::function<void()> callback) { 
    sourceData->selectObject(data_config, callback); }

void selectAllObjects(std::function<void()> callback) { 
    sourceData->selectAllObjects(callback); }

void insertObject(DataConfig data_config, std::function<void()> callback) { 
    sourceData->insertObject(data_config, callback); }

void updateObject(DataConfig data_config, std::function<void()> callback) { 
    sourceData->updateObject(data_config, callback); }
};