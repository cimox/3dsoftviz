#include "Kinect/KinectZoom.h"

Kinect::KinectZoom::KinectZoom()
{

}

Kinect::KinectZoom::~KinectZoom()
{

}

void Kinect::KinectZoom::zoom(cv::Mat frame,openni::VideoStream *m_depth, float x, float y, float z)
{
	openni::CoordinateConverter coordinateConverter;
	float x1;
	float y1;
	float x2;
	float y2;
	float z1;
	coordinateConverter.convertWorldToDepth(*m_depth, x-90.0,y-90.0,z, &x1, &y1, &z1);
	coordinateConverter.convertWorldToDepth(*m_depth, x+110.0,y+130.0,z, &x2, &y2, &z1);

	openni::VideoFrameRef depthFrame;
	m_depth->readFrame(&depthFrame);
	//openni::DepthPixel *depthPixels = new DepthPixel[depthFrame.getHeight()*depthFrame.getWidth()];
	openni::DepthPixel* depthPixels = (openni::DepthPixel*)depthFrame.getData();
	//std::memcpy(depthPixels, depthFrame.getData(), depthFrame.getHeight()*depthFrame.getWidth()*sizeof(uint16_t));
	cv::Mat depthImage(depthFrame.getHeight(), depthFrame.getWidth(), CV_16UC1, depthPixels);


	cv::Rect rect;
	rect.x = (int) x1;
	rect.y = frame.rows - (int) y1;
	rect.width = abs((int)(x1-x2));
	rect.height = abs((int)(y1-y2));
	if (rect.x<0) rect.x=0;
	if (rect.y<0) rect.y=0;
	if (rect.width<0) rect.width=0;
	if (rect.height<0) rect.height=0;
	if ((rect.x+rect.width)>frame.cols-1) rect.width =frame.cols-1-rect.x;
	if ((rect.y+rect.height)>frame.rows-1) rect.height = frame.rows-1-rect.y;
	std::cout << x <<" " << y << " " << z << std::endl;
	std::cout << rect.x << " " << rect.y << " " << rect.width << " " << rect.height << std::endl;
	cv::Mat subImg = frame(rect);
	depthImage = depthImage(rect);
	//cvtColor(depthImage,depthImage,CV_BGR2GRAY);
	double minVal;
	double maxVal;
	cv::Point minLoc;
	cv::Point maxLoc;

	minMaxLoc( depthImage, &minVal, &maxVal, &minLoc, &maxLoc );

	cv::Mat depthImage2;
	depthImage.convertTo(depthImage2,CV_8UC1,255/maxVal);
	cv::Scalar mean = cv::mean(depthImage2(cv::Rect(depthImage2.cols/2-5,depthImage2.rows/2-5,10,10)));
	//cv::Mat mask(cv::Size(depthImage.cols+40,depthImage.rows+40,CV_8UC1);
	double low = mean[0]-1; if (low<0) low=0;
	double high = mean[0]+1; if (high>255) high=255;
	std::cout<<mean[0]<<" mean" <<std::endl;
	Mat depthImage3 = depthImage2.clone();
	//cv::floodFill(depthImage3, cv::Point(depthImage3.cols/2,depthImage3.rows/2),
	//			  cv::Scalar(255) ,0, cv::Scalar(low),cv::Scalar(high));
	cv::Mat mask = cv::Mat::zeros(depthImage3.rows + 2, depthImage3.cols + 2, CV_8U);
	cv::floodFill(depthImage3, mask, cv::Point(depthImage3.cols/2,depthImage3.rows/2),
				  255, 0, cv::Scalar(2),
				  cv::Scalar(2),  4 + (255 << 8) + cv::FLOODFILL_MASK_ONLY);
	cv::Mat binary,binary2;
	//cv::adaptiveThreshold(gray,binary,255,CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY,11,2);
	//threshold( gray, binary, cv::mean( gray ).val[0], 255,0 );
	cv::blur(depthImage2,binary,cv::Size(5,5));
	cv::blur(depthImage2,binary2,cv::Size(5,5));
	cv::blur(mask,mask,cv::Size(3,3));
	cv::bitwise_not(binary2,binary2);
	cv::threshold(binary,binary,(int)mean[0]-15,255,CV_THRESH_BINARY);
	cv::threshold(binary2,binary2,255-(int)mean[0]-1,255,CV_THRESH_BINARY);

	cv::namedWindow( "testing", CV_WINDOW_AUTOSIZE );
	cv::namedWindow( "depth", CV_WINDOW_AUTOSIZE );
	cv::namedWindow( "binary", CV_WINDOW_AUTOSIZE );
	cv::namedWindow( "binary2", CV_WINDOW_AUTOSIZE );
	cv::namedWindow( "floodfill", CV_WINDOW_AUTOSIZE );
	cv::imshow("testing",subImg);
	cv::imshow("depth",depthImage2);
	cv::imshow("binary", binary);
	cv::imshow("binary2", binary2);
	cv::imshow("floodfill", mask);
	cv::waitKey(33);
}
