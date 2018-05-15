%% CSL 461 Final Project
%% Create Pointillism Art from Digital Images
%  Dependency: dispstat.m, stroke1-4.jpg
%% Note: 
%  (1) The algorithm has been adapted from: 
%      https://web.stanford.edu/class/ee368/Project_Autumn_1516/Reports/Hong_Liu.pdf   
%% Developed By: 
%  Raghav Sharma
%  2015EEB1071 
%%

% Step 1: load, blur and downsample the image
img = im2double(imread('image/balloons.jpg'));
scale = 5;
imgPreprocessed = imresize(img, 1/scale, 'Antialiasing', true);
dispstat1('Step 1 Done', 'keepthis');

% Step 2: K-means clustering algorithm to find K primary colors
K = 8;
imgThumbnail = imresize(imgPreprocessed, 1/3.0);
imgThumbnail(imgThumbnail < 0) = 0;
[rows, cols, ~] = size(imgThumbnail);

% initialize centroids to randomly chosen pixels from the image
centroids = zeros(K, 3);

for i = 1:K
    randRow = randi([1 rows]);
    randCol = randi([1 cols]);
    centroids(i, :) = imgThumbnail(randRow, randCol, :);
end

assignment = zeros(rows, cols);
finalCost = 0;
iter = 0;

% find optimal assignments with respect to current centroids
while 1
    totalCost = 0;
    for i = 1:rows
        for j = 1:cols
            pixel = squeeze(imgThumbnail(i, j, :))';
            distances = sum((repmat(pixel, K, 1) - centroids) .^ 2, 2);
            [minDistance, index] = min(distances);
            totalCost = totalCost + minDistance;
            assignment(i, j) = index;
        end
    end
    
    if abs(totalCost-finalCost) < 0.001
        break
    end
    
    finalCost = totalCost;
    
    % update centroids
    for index = 1:K
        clusterSum = zeros(1, 3);
        clusterN = 0;
        for i = 1:rows
            for j = 1:cols
                if assignment(i, j) == index
                    clusterSum = clusterSum + squeeze(imgThumbnail(i, j, :))';
                    clusterN = clusterN + 1;
                end
            end
        end
        
        if clusterN > 0
            centroids(index, :) = clusterSum ./ clusterN;
        end
    end
    iter = iter + 1;
    dispstat1(sprintf('Iteration: %d, Cost: %.4f', iter, totalCost));
end

% boost color saturation
tempImage = zeros(K, 1, 3);
tempImage(:, 1, :) = centroids;
tempHSV = rgb2hsv(tempImage);

tempHSV(:, :, 1) = tempHSV(:, :, 1);
tempHSV(:, :, 2) = tempHSV(:, :, 2) .^ 0.75 + 0.05;
tempHSV(:, :, 3) = tempHSV(:, :, 3) .^ 0.75 + 0.05;

tempHSV = repmat(tempHSV, [2 1 1]);
tempHSV(K+1:2*K, :, 1) = mod(tempHSV(K+1:2*K, :, 1) + randn(K, 1) * 0.5, 1.0);
tempImage = hsv2rgb(tempHSV);

centroids = squeeze(tempImage(:, 1, :));
K = 2*K;

centroids = centroids*0.9 + 0.05;

dispstat1('Step 2 Done', 'keepthis');

% Step 3: Convert image to distribution of dots

% find the reversed intensity map (white = 0)
imgGreyscale = rgb2gray(imgPreprocessed);
intensity = 1 - imgGreyscale;
intensity = intensity .^ 1.8 + 0.05;

imgConverted = imgPreprocessed;
[row, col, ~] = size(imgConverted);
imgPrimarycolors = zeros(row, col, 3);

for i = 1:row
    for j = 1:col
        % RGB color value
        pixel = squeeze(imgConverted(i, j, :))';
        
        % determine which 3 colors from the pallete will compose pixel
        distances = sum((repmat(pixel, K, 1) - centroids) .^ 2, 2);
        [~, index] = sort(distances);
        index = index([1 2 randperm(K - 2) + 2]);
        
        % each pixel's 3 primary colors are stored in imgPrimarycolors
        imgPrimarycolors(i, j, :) = index(1:3);
        primaryColormatrix = [centroids(index(1), :);
                              centroids(index(2), :);
                              centroids(index(3), :)];
                          
        imgConverted(i, j, :) = squeeze(imgConverted(i, j, :))' / primaryColormatrix;
    end
end

dispstat1('Step 3 Done', 'keepthis');

% Step 4: normalize dot distribution

% contrast enhancement
imgEqualized = imgConverted + 0.001;
imgEqualized(imgEqualized < 0) = 0;
imgEqualized = imgEqualized .^ 1.5;

% calculate number of dots per pixel
numDots = imgEqualized;
alpha = 100.0;

for i = 1:row
    for j = 1:col
        multiplier = alpha * intensity(i, j) / sum(numDots(i, j, :));
        numDots(i, j, :) = round(squeeze(numDots(i, j, :)) .* multiplier);
    end
end

numDots = uint8(numDots);
dispstat1('Step 4 Done', 'keepthis');

% Step 5: Edge detection for orientation of strokes
kirsch = cell(1, 4);

% horizontal
kirsch{1} = [5 5 5; -3 0 -3; -3 -3 -3];

% main diagonal
kirsch{2} = [-3 5 5; -3 0 5; -3 -3 -3];

% vertical
kirsch{3} = [-3 -3 5; -3 0 5; -3 -3 5];

% secondary diagonal
kirsch{4} = [-3 -3 -3; -3 0 5; -3 5 5];

convolvedImages = zeros(row, col, 4);
convolvedImages = abs(convolvedImages);

imgOrientation = zeros(row, col);

for i = 1:4
    convolvedImages(:, :, i) = imfilter(imgGreyscale, kirsch{i}, 'conv');
end

convolvedImages = abs(convolvedImages);

imgOrientation = zeros(row, col);

for i = 1:row
    for j = 1:col
        [v, imgOrientation(i, j)] = max(squeeze(convolvedImages(i, j, :)));
        if v < 2.0
            % if region is flat, use the standard orientation (secondary diagonal)
            imgOrientation(i, j) = 4;
        end
    end
end

dispstat1('Step 5 Done', 'keepthis');

% Step 6: scatter dots into canvas

% d: space between each dot cluster
% sigma and mu are parameters of a gaussian kernel

d = 25.0;
sigma = 25.0;
mu = 10.0;

% blockSize is for reference bar
blockSize = floor(round(row*d) * 0.6 / K);
canvas = ones(round(row * d), round(col * d) + blockSize + 10, 3);

% canvas is larger than downsampled image
[newRow, newCol, ~] = size(canvas);

% radius is the size of the dot kernel (stroke)
radius = 6.0;
dotKernel = cell(1, 4);

for i = 1:4
    dotKernel{i} = (1.0 - rgb2gray(im2double(imread(sprintf('stroke%d.jpg', i))))) .* 0.4;
end

for i = 1:row
    for j = 1:col
        dispstat1(sprintf('Processing: Row %d/%d', i, row));
        
        totalDots = sum(numDots(i, j, :));
        
        color = [ones(1, numDots(i, j, 1)) * imgPrimarycolors(i, j, 1) ...
                 ones(1, numDots(i, j, 2)) * imgPrimarycolors(i, j, 2) ...
                 ones(1, numDots(i, j, 3)) * imgPrimarycolors(i, j, 3)];
             
        color = color(randperm(totalDots));
        
        distance = randn(1, totalDots) * sigma + mu;
        angle = rand(1, totalDots) * 2 * pi;
        
        xDelta = distance .* cos(angle);
        yDelta = distance .* sin(angle);
        
        x = round((j-0.5) * d + xDelta);
        y = round((i-0.5) * d + yDelta);
        
        for k = 1:numel(x)
            x0 = x(1, k);
            y0 = y(1, k);
            
            if (x0 >= radius) && (x0 <= newCol - (radius - 1)) && (y0 >= radius) && (y0 <= newRow - (radius - 1))
                orientedDotkernel = dotKernel{imgOrientation(i, j)};
                
                for channel = 1:3
                    canvas(y0-(radius-1):y0+(radius-1), x0-(radius-1):x0+(radius-1), channel) = (1-orientedDotkernel) .* canvas(y0-(radius-1):y0+(radius-1), x0-(radius-1):x0+(radius-1), channel) + orientedDotkernel .* centroids(color(k), channel);
                end
            end
        end
    end
end

% generate color card
colorCard = zeros(blockSize*K, blockSize, 3);

for i = 1:blockSize*K
    for j = 1:blockSize
        for k = 1:3
            colorCard(i, j, k) = centroids(floor((i-1)/blockSize) + 1, k);
        end
    end
end

imwrite(colorCard, 'result/colorCard.png');

% append color card to the right edge of canvas
serialCounter = 0;
[colorCardlength, colorCardwidth, ~] = size(colorCard);
canvas(5:5+colorCardlength-1, end-colorCardwidth-4:end-5, :) = colorCard;

serialCounter = serialCounter + 1;
dispstat1('Saving...');

imwrite(canvas, sprintf('result/img_%d.png', serialCounter));
dispstat1('Step 6 Done', 'keepthis');

      