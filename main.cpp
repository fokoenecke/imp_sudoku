#include <opencv2/core/core.hpp>
#include <fstream>
#include "Sudoku.h"
#include "SVMTrainer.h"

using namespace cv;


int test() {

	Mat sourceImage = imread("img/digit_5.png", CV_LOAD_IMAGE_GRAYSCALE);
    Mat debugImage = imread("img/digit_5.png", CV_LOAD_IMAGE_COLOR);
	std::cout << "source image - rows: " << sourceImage.rows << " cols: " << sourceImage.cols << std::endl;

	Mat binaryImage;
    bitwise_not ( sourceImage, sourceImage);
	cv::threshold(sourceImage, binaryImage, 100, 255, cv::THRESH_BINARY);

	Mat croppedImage;

	const char* windowNameSrc = "Source Image";
	const char* windowNameBinary = "Binary";
	const char* windowNameDebug = "Debug";
	const char* windowNameCropped = "Cropped";





	CvSVM svm;
	svm.load("svm/5_test.xml");





	// Konturen finden
	std::vector<std::vector<cv::Point> > contours;

	// Für das Finden der Konturen sollte ein eigenes Mat-Objekt erzeugt werden,
	// da die Funktion findContours(...) das Mat-Objekt für die Konturberechnungen verändert.
	cv::Mat contourMat = binaryImage.clone();

	cv::findContours(contourMat, contours, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);

	// Gibt die Anzahl der gefundenen Konturen in der Konsole aus
	std::cout << "contours size: " << contours.size() << std::endl;





	// Flächenmäßig größte Kontur ermitteln

	// Speichert den Index der größten Kontur ab
	int largestContourIndex = 0;

	// Speichert die Fläche der größten Kontur ab
	double largestContourArea = 0;

	if (contours.size() > 0)
	{
		// Über alle gefundenen Konturen laufen
		for( int i = 0; i< contours.size(); i++ )
		{
			// Fläche der Kontur berechnen
			double contourArea = cv::contourArea(contours[i], false);

			// Überprüfen, ob aktuelle Konturfläche größer ist,
			// als die bisher gespeicherte.
			if (contourArea > largestContourArea)
			{
				largestContourArea = contourArea;
				largestContourIndex = i;
			}
		}
	}

	std::cout << "largestContourIndex: " << largestContourIndex << " with size: " << largestContourArea << std::endl;

	// Kontur einzeichnen
	cv::drawContours(debugImage, contours, largestContourIndex, CV_RGB(0, 120, 0), 1, 8);



	Rect rect = boundingRect(contours[largestContourIndex]);
	Point pt1, pt2;
	pt1.x = rect.x;
	pt1.y = rect.y;
	pt2.x = rect.x + rect.width;
	pt2.y = rect.y + rect.height;

	cv::rectangle(debugImage, pt1, pt2, CV_RGB(255,0,0), 1);

	// Wir schneiden jetzt den Bereich des berechneten "bounding rectangles" aus dem
	// Mat-Objekt "binaryImage" aus und speichern das Ergebnis in das Mat-Objekt "croppedImage"
	// Dies kann in OpenCV mit folgender Kurzschreibweise durchgeführt werden.
	croppedImage = binaryImage(rect);



	// "Normale" Momente aus der größten gefundenen Kontur berechnen
	cv::Moments moments = cv::moments(contours[largestContourIndex]);

	// Es gibt 7 Hu-Momente, welche in diesem Array gespeichert werden.
	double huMoments[7];

	// Hu-Momente aus den bereits gefundenen "normalen" Momenten berechnen
    cv::HuMoments(moments, huMoments);

	// Hinweis: Die Funktion HuMoments(...) sorgt dafür, dass das Array "huMoments" jetzt alle 7 Hu-Momente gespeichert hat.




	// Hu-Momente in ein eigenes Mat-Objekt schreiben
	// Die SVM erwartet ein Mat-Objekt als Parameter
    cv::Mat extractedFeatures(1, 7, CV_32FC1);

	for(int i = 0; i < 7; i++)
	{
		extractedFeatures.at<float>(0, i) = huMoments[i];
	}





	int detectedClass = svm.predict(extractedFeatures);

	std::cout << "SVM - detected class: " << detectedClass << std::endl;





	// Erkannte Klasse in das Mat-Objekt "debugImage" schreiben

	// Diese Routine verbindet die beiden Variablen “detectedClass” und „text“ zu einem string-Objekt,
    // welches dann in das Mat-Objekt gezeichnet werden soll.
	std::stringstream sstm;
	string text = "Ziffer: ";
	sstm << text << detectedClass;
	string result = sstm.str();

    // Hier werden Schriftart und Schriftgröße festgelegt.
	int fontFace = FONT_HERSHEY_PLAIN;
	double fontScale = 1;
	int thickness = 1;

    // Ursprung für das Zeichnen des Texts im Mat-Objekt
	cv::Point textOrigin(0, debugImage.rows);

    // Die Funktion cv::putText(…) zeichnet ein string-Objekt in das angegebene Mat-Objekt.
	cv::putText(debugImage, result, textOrigin, fontFace, fontScale, Scalar(255, 0, 0), thickness, 8);

	// Ergebnisse anzeigen lassen
	//namedWindow(windowNameSrc);
	//moveWindow(windowNameSrc, 0, 0);
	//namedWindow(windowNameBinary);
	//moveWindow(windowNameBinary, sourceImage.cols, 0);
	namedWindow(windowNameDebug);
    moveWindow(windowNameDebug, 0, sourceImage.rows);
	//namedWindow(windowNameCropped);
	//moveWindow(windowNameCropped, sourceImage.cols, sourceImage.rows);

	//imshow(windowNameSrc, sourceImage);
	//imshow(windowNameBinary, binaryImage);
	imshow(windowNameDebug, debugImage);
	//imshow(windowNameCropped, croppedImage);
}

int reverseInt(int i) {
    unsigned char c1, c2, c3, c4;
    c1 = i & 255;
    c2 = (i >> 8) & 255;
    c3 = (i >> 16) & 255;
    c4 = (i >> 24) & 255;
    return ((int)c1 << 24) + ((int)c2 << 16) + ((int)c3 << 8) + c4;
}


int test_read_hog() {

    ifstream image_file ("MNIST/train-images.idx3-ubyte",ios::binary);
    ifstream label_file ("MNIST/train-labels.idx1-ubyte",ios::binary);

    if (image_file.is_open() && label_file.is_open()) {

        int magic_number=0; int number_of_images=0; int row; int col;
        int n_rows=0; int n_cols=0; unsigned char label=0; unsigned char pixel=0;

        image_file.read((char*)&magic_number,sizeof(magic_number));
        label_file.read((char*)&magic_number,sizeof(magic_number));
        magic_number = reverseInt(magic_number);

        image_file.read((char*)&number_of_images,sizeof(number_of_images));
        label_file.read((char*)&number_of_images,sizeof(number_of_images));
        number_of_images = reverseInt(number_of_images);

        Mat trainingData(0, 32, CV_32FC1);
        Mat labels(0, 1, CV_32FC1);

        image_file.read((char*)&n_rows,sizeof(n_rows));
        n_rows = reverseInt(n_rows);
        image_file.read((char*)&n_cols,sizeof(n_cols));
        n_cols = reverseInt(n_cols);

        unsigned char image_arr[28*28];
        for (int image_index=0; image_index<number_of_images; ++image_index) {

            label_file.read((char*)&label,sizeof(label));

            for (row=0; row<n_rows; ++row) {
                for(col=0; col<n_cols; ++col) {
                    image_file.read((char*)&pixel,sizeof(pixel));
                    image_arr[(row*28)+col]= (int)pixel;
                }
            }

            Mat image = Mat(1, 28*28, CV_32FC1, &image_arr);
            cout << "training data..." << endl;
            trainingData.push_back(image);
            labels.push_back((int)label);

        }

        //CvParamGrid CvParamGrid_C(pow(2.0,-5), pow(2.0,15), pow(2.0,2));
        //CvParamGrid CvParamGrid_gamma(pow(2.0,-15), pow(2.0,3), pow(2.0,2));

        CvSVMParams params;
        params.svm_type    = CvSVM::C_SVC;
        params.kernel_type = CvSVM::RBF;
        params.term_crit   = cvTermCriteria(CV_TERMCRIT_ITER, 1000, 1e-6);

        CvSVM SVM;

        //SVM.train_auto(trainingData, labels, Mat(), Mat(), params, 10, CvParamGrid_C, CvParamGrid_gamma, CvSVM::get_default_grid(CvSVM::P), CvSVM::get_default_grid(CvSVM::NU), CvSVM::get_default_grid(CvSVM::COEF), CvSVM::get_default_grid(CvSVM::DEGREE), true);
        SVM.train(trainingData, labels, Mat(), Mat(), params);
        params = SVM.get_params();
        cout<<"gamma:"<<params.gamma<<endl; // 0.125
        cout<<"C:"<<params.C<<endl;         // 0.03125

        SVM.save("svm/5_test.xml");

    }

}


int test_read() {

    ifstream image_file ("MNIST/train-images.idx3-ubyte",ios::binary);
    ifstream label_file ("MNIST/train-labels.idx1-ubyte",ios::binary);

    if (image_file.is_open() && label_file.is_open()) {

        int magic_number=0; int number_of_images=0; int row; int col;
        int n_rows=0; int n_cols=0; unsigned char label=0; unsigned char pixel=0;

        image_file.read((char*)&magic_number,sizeof(magic_number));
        label_file.read((char*)&magic_number,sizeof(magic_number));
        magic_number = reverseInt(magic_number);

        image_file.read((char*)&number_of_images,sizeof(number_of_images));
        label_file.read((char*)&number_of_images,sizeof(number_of_images));
        number_of_images = reverseInt(number_of_images);

        Mat trainingData(0, 7, CV_32FC1);
        Mat labels(0, 1, CV_32FC1);

        image_file.read((char*)&n_rows,sizeof(n_rows));
        n_rows = reverseInt(n_rows);
        image_file.read((char*)&n_cols,sizeof(n_cols));
        n_cols = reverseInt(n_cols);

        unsigned char image_arr[28][28];
        for (int image_index=0; image_index<number_of_images; ++image_index) {

            label_file.read((char*)&label,sizeof(label));

            for (row=0; row<n_rows; ++row) {
                for(col=0; col<n_cols; ++col) {
                    image_file.read((char*)&pixel,sizeof(pixel));
                    image_arr[row][col]= (int)pixel;
                }
            }

            Mat binaryImage, debugImage;
            Mat image = Mat(28, 28, CV_8UC1, &image_arr);
            cv::cvtColor(image, debugImage, CV_GRAY2BGR);
            threshold(image, binaryImage, 100, 255, THRESH_BINARY);

            vector<vector<Point> > contours;
            Mat contourMat = binaryImage.clone();
            findContours(contourMat, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);
            int largestContourIndex = 0;

            double largestContourArea = 0;
            if (contours.size() > 0) {
                for( int i = 0; i<contours.size(); i++ ) {
                    double tmpContourArea = contourArea(contours[i], false);
                    if (tmpContourArea > largestContourArea) {
                        largestContourArea = tmpContourArea;
                        largestContourIndex = i;
                    }
                }
            }

            drawContours(debugImage, contours, largestContourIndex, CV_RGB(255, 0, 0), 1, 8);
            Moments digit_moments = moments(contours[largestContourIndex]);
            double huMoments[7] = {0};
            HuMoments(digit_moments, huMoments);

            Mat features(1, 7, CV_32FC1);
            for (int i=0; i<7; i++) {
                features.at<float>(0, i) = huMoments[i];
            }

            trainingData.push_back(features);
            labels.push_back((int)label);

        }

        //CvParamGrid CvParamGrid_C(pow(2.0,-5), pow(2.0,15), pow(2.0,2));
        //CvParamGrid CvParamGrid_gamma(pow(2.0,-15), pow(2.0,3), pow(2.0,2));

        CvSVMParams params;
        params.svm_type    = CvSVM::C_SVC;
        params.kernel_type = CvSVM::RBF;
        params.term_crit   = cvTermCriteria(CV_TERMCRIT_ITER, 1000, 1e-6);
        params.gamma       = 20000; // 20000
        params.C           = 0.00001; //0.00001 => ~5400

        CvSVM SVM;

        //SVM.train_auto(trainingDataMat, labelsMat, Mat(), Mat(), params, 10, CvParamGrid_C, CvParamGrid_gamma, CvSVM::get_default_grid(CvSVM::P), CvSVM::get_default_grid(CvSVM::NU), CvSVM::get_default_grid(CvSVM::COEF), CvSVM::get_default_grid(CvSVM::DEGREE), true);
        SVM.train(trainingData, labels, Mat(), Mat(), params);
        params = SVM.get_params();

        cout<<"gamma:"<<params.gamma<<endl; // 0.125
        cout<<"C:"<<params.C<<endl;         // 0.03125

        SVM.save("svm/5_test.xml");

    }
}

int test_predict() {

    ifstream image_file ("MNIST/t10k-images.idx3-ubyte",ios::binary);
    ifstream label_file ("MNIST/t10k-labels.idx1-ubyte",ios::binary);

    if (image_file.is_open() && label_file.is_open()) {

        CvSVM svm;
        svm.load("svm/5_test.xml");

        int magic_number=0; int number_of_images=0; int row; int col;
        int n_rows=0; int n_cols=0; unsigned char label=0; unsigned char pixel=0;

        image_file.read((char*)&magic_number,sizeof(magic_number));
        label_file.read((char*)&magic_number,sizeof(magic_number));
        magic_number = reverseInt(magic_number);

        image_file.read((char*)&number_of_images,sizeof(number_of_images));
        label_file.read((char*)&number_of_images,sizeof(number_of_images));
        number_of_images = reverseInt(number_of_images);

        cout << number_of_images << endl;

        Mat trainingDataMat(number_of_images, 7, CV_32FC1);
        Mat labelsMat(number_of_images, 1, CV_32FC1);

        image_file.read((char*)&n_rows,sizeof(n_rows));
        n_rows= reverseInt(n_rows);
        image_file.read((char*)&n_cols,sizeof(n_cols));
        n_cols= reverseInt(n_cols);

        //namedWindow("debug");
        //moveWindow("debug", 100, 100);

        int rightCount = 0;
        unsigned char image_arr[28][28];
        for (int image_index=0; image_index<number_of_images; ++image_index) {

            label_file.read((char*)&label,sizeof(label));

            for (row=0; row<n_rows; ++row) {
                for(col=0; col<n_cols; ++col) {
                    image_file.read((char*)&pixel,sizeof(pixel));
                    image_arr[row][col]= (int)pixel;
                }
            }

            Mat binaryImage, debugImage;
            Mat image = Mat(28, 28, CV_8UC1, &image_arr);
            cv::cvtColor(image, debugImage, CV_GRAY2BGR);
            threshold(image, binaryImage, 100, 255, THRESH_BINARY);

            vector<vector<Point> > contours;
            Mat contourMat = binaryImage.clone();
            findContours(contourMat, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);
            int largestContourIndex = 0;

            double largestContourArea = 0;
            if (contours.size() > 0) {
                for( int i = 0; i<contours.size(); i++ ) {
                    double tmpContourArea = contourArea(contours[i], false);
                    if (tmpContourArea > largestContourArea) {
                        largestContourArea = tmpContourArea;
                        largestContourIndex = i;
                    }
                }
            }

            drawContours(debugImage, contours, largestContourIndex, CV_RGB(255, 0, 0), 1, 8);
            Moments digit_moments = moments(contours[largestContourIndex]);
            double huMoments[7] = {0};
            HuMoments(digit_moments, huMoments);

            float features[7] = {0};
            for(int i = 0; i < 7; i++) {
                features[i] = huMoments[i];
            }
            Mat extractedFeatures(1, 7, CV_32FC1, features);
            int detectedClass = svm.predict(extractedFeatures);
            //std::cout << "SVM - detected class: " << detectedClass << std::endl;

            if (detectedClass == (int)label) {
                rightCount++;
            }

        }
        cout << "(" << rightCount << "/" << number_of_images << ")" << endl;

    }
}

int test_predict_hog() {

    ifstream image_file ("MNIST/t10k-images.idx3-ubyte",ios::binary);
    ifstream label_file ("MNIST/t10k-labels.idx1-ubyte",ios::binary);

    if (image_file.is_open() && label_file.is_open()) {

        CvSVM svm;
        svm.load("svm/5_test.xml");

        int magic_number=0; int number_of_images=0; int row; int col;
        int n_rows=0; int n_cols=0; unsigned char label=0; unsigned char pixel=0;

        image_file.read((char*)&magic_number,sizeof(magic_number));
        label_file.read((char*)&magic_number,sizeof(magic_number));
        magic_number = reverseInt(magic_number);

        image_file.read((char*)&number_of_images,sizeof(number_of_images));
        label_file.read((char*)&number_of_images,sizeof(number_of_images));
        number_of_images = reverseInt(number_of_images);

        cout << number_of_images << endl;

        Mat trainingDataMat(number_of_images, 32, CV_32FC1);
        Mat labelsMat(number_of_images, 1, CV_32FC1);

        image_file.read((char*)&n_rows,sizeof(n_rows));
        n_rows= reverseInt(n_rows);
        image_file.read((char*)&n_cols,sizeof(n_cols));
        n_cols= reverseInt(n_cols);

        vector<float> descriptors;

        int rightCount = 0;
        unsigned char image_arr[28*28];
        for (int image_index=0; image_index<number_of_images; ++image_index) {

            label_file.read((char*)&label,sizeof(label));

            for (row=0; row<n_rows; ++row) {
                for(col=0; col<n_cols; ++col) {
                    image_file.read((char*)&pixel,sizeof(pixel));
                    image_arr[(row*28)+col]= (int)pixel;
                }
            }

            Mat binaryImage, debugImage;
            Mat image = Mat(1, 28*28, CV_32FC1, &image_arr);
            int detectedClass = svm.predict(image);
            //std::cout << "SVM - detected class: " << detectedClass << std::endl;

            if (detectedClass == (int)label) {
                rightCount++;
            }

        }
        cout << "(" << rightCount << "/" << number_of_images << ")" << endl;

    }
}

int main(int argc, char *argv[]) {

    //SVMTrainer trainer;
    //trainer.train();

    //test();
    //test_read_hog();
    //test_predict_hog();

    Mat img = imread("img/sudoku_005.png", CV_LOAD_IMAGE_COLOR);
    if(img.empty())
       return -1;

    Sudoku sudoku = Sudoku(img);
    sudoku.detect();

    waitKey();
    return 0;
}



