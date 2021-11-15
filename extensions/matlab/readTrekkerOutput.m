function tractogram = readTrekkerOutput(vtk_fname)

fid              = fopen(vtk_fname,'r','s','UTF-8');

% Read vtk header
vtkVersion       = fgetl(fid);
trekkerVersion   = fgetl(fid);
fileType         = fgetl(fid);

fgetl(fid);         % dummy read of 'DATASET POLYDATA'
fscanf(fid,'%s',1); % dummy read of 'POINTS'

pointCount       = fscanf(fid,'%d',1);
fscanf(fid,'%s',1);  % dummy read of 'float'
fread(fid,1,'char'); % dummy read of end of line

if (strcmp(fileType,'ASCII'))
    
    % Read points
    points           = fscanf(fid,'%f',3*pointCount);
    points           = reshape(points,[3,pointCount])';
    
    fscanf(fid,'%s',1); % dummy read
    lineCount       = fscanf(fid,'%d',1);
    fscanf(fid,'%d',1); % dummy read
    idxEnds        	= zeros(lineCount,1);
    for i=1:lineCount
        length      = fscanf(fid,'%d',1);
        fscanf(fid,'%d',length); % dummy read
        if (i==1)
            idxEnds(1) = length;
        else
            idxEnds(i) = idxEnds(i-1) + length;
        end
    end
    
else
    
    % Read points
    points           = fread(fid,3*pointCount,'float32');
    points           = reshape(points,[3,pointCount])';
    
    fscanf(fid,'%s',1); % dummy read of 'LINES'
    lineCount       = fscanf(fid,'%d',1);
    fscanf(fid,'%d',1); % dummy read of size
    fread(fid,1,'char'); % dummy read of end of line
    idxEnds        	= zeros(lineCount,1);
    
    for i=1:lineCount
        length       = fread(fid,1,'int32');
        fread(fid,length,'int32'); % dummy read point indices
        if (i==1)
            idxEnds(1) = length;
        else
            idxEnds(i) = idxEnds(i-1) + length;
        end
    end
    
    try
        
        fscanf(fid,'%s',1); % dummy read of 'CELL_DATA'
        fscanf(fid,'%s',1); % dummy read number of cells, i.e., lines
        fscanf(fid,'%s',1); % dummy read of 'SCALARS'
        
        cellDataName    = fscanf(fid,'%s',1);
        seedCoordinates = [];
        
        if (strcmp(cellDataName,'seedCoordinates'))
            fgetl(fid); % dummy read of ' float 3'
            fgetl(fid); % dummy read of 'LOOKUP_TABLE default'
            seedCoordinates  = fread(fid,3*lineCount,'float32');
            seedCoordinates  = reshape(seedCoordinates,[3,lineCount])';
        end
        
    catch
    end
    
end


tractogram.points           = points;
tractogram.idxEnds          = idxEnds;
tractogram.seedCoordinates  = seedCoordinates;

fclose(fid);
