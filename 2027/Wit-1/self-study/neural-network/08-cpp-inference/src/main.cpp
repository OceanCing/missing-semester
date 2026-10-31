#include "DigitNet.hpp"
#include <chrono>
#include <fstream>
#include <iostream>
#include <thread>

int main(int argc,char** argv) {
  cv::CommandLineParser parser(argc,argv,"{models||}{assets||}{output||}{config||}{input||}{mode|infer|}"
    "{preprocess|normal|}{headless|false|}{roi|false|}{repeat|100|}{value|60|}{threshold|.65|}");
  const auto models=parser.get<std::string>("models"),assets=parser.get<std::string>("assets");
  const auto output=parser.get<std::string>("output"),mode=parser.get<std::string>("mode");
  const bool headless=parser.get<bool>("headless");
  const float threshold=parser.get<float>("threshold");
  if(!parser.check() || models.empty() || output.empty() || !std::isfinite(threshold) || threshold<0 || threshold>1) return 2;
  try {
    cv::setNumThreads(1);
    lab::mkdir(output);
    nncourse::DigitNet net(models);
    cv::FileStorage results(output+"/metrics.json",cv::FileStorage::WRITE|cv::FileStorage::FORMAT_JSON);
    results<<"opencv_version"<<CV_VERSION;
    if(mode=="parity") {
      cv::FileStorage fixtures(models+"/parity.json",cv::FileStorage::READ);
      double maximum=0; int count=0;
      for(const auto& item:fixtures["cases"]) {
        const auto image=cv::imread(models+"/"+static_cast<std::string>(item["image"]));
        const auto actual=net.infer(image,static_cast<int>(item["roi"])!=0);
        int index=0;
        for(const auto& expected:item["logits"]) {
          maximum=std::max(maximum,std::abs(actual.logits.at(index++)-static_cast<double>(expected)));
        }
        if(index!=static_cast<int>(actual.logits.size())) throw std::runtime_error("Fixture output size mismatch");
        ++count;
      }
      results<<"cases"<<count<<"maximum_logit_difference"<<maximum;
      std::cout<<"C++ PARITY cases="<<count<<" max_difference="<<maximum<<std::endl;
      if(count<12 || maximum>.001) throw std::runtime_error("C++/PyTorch parity failed");
    } else if(mode=="infer") {
      auto input=parser.get<std::string>("input");
      if(input.empty()) input=models+"/fixtures/digit-07.png";
      const auto image=cv::imread(input);
      nncourse::Classification c;
      for(int i=0;i<10;++i) c=net.infer(image,parser.get<bool>("roi"),threshold,parser.get<std::string>("preprocess"));
      const int repeats=parser.get<int>("repeat");
      if(repeats<1) throw std::runtime_error("repeat must be positive");
      const auto begin=std::chrono::steady_clock::now();
      for(int i=0;i<repeats;++i) c=net.infer(image,parser.get<bool>("roi"),threshold,parser.get<std::string>("preprocess"));
      double ms=std::chrono::duration<double,std::milli>(std::chrono::steady_clock::now()-begin).count()/repeats;
      results<<"class_id"<<c.id<<"class_name"<<c.name<<"score"<<c.score<<"accepted"<<int(c.accepted)<<"mean_inference_ms"<<ms;
      std::cout<<"class="<<c.name<<" score="<<c.score<<" mean_ms="<<ms<<std::endl;
      cv::Mat display,color;
      if(image.channels()==1)cv::cvtColor(image,color,cv::COLOR_GRAY2BGR);else color=image;
      cv::resize(color,display,{400,400},0,0,cv::INTER_NEAREST);
      cv::putText(display,"class="+c.name+" score="+std::to_string(c.score).substr(0,4),{10,25},cv::FONT_HERSHEY_SIMPLEX,.55,{0,220,255},1);
      cv::imwrite(output+"/overview.png",display);
      if(!headless){cv::imshow("08 C++ inference",display);while(true){int k=cv::waitKey(30)&255;if(k=='q'||k==27||lab::closed("08 C++ inference"))break;}}
    } else if(mode=="video") {
      const auto config=lab::loadConfig(parser.get<std::string>("config"));
      std::ofstream table(output+"/frames.csv");
      table<<"frame,roi_valid,classified,class_id,score,has_pose,z_m,processing_ms\n";
      int classified=0,valid=0,frames=0;
      cv::Mat last;
      const std::string window="09 Digit video: Space pause, Q exit";
      if(!headless) cv::namedWindow(window,cv::WINDOW_NORMAL);
      for(int i=0;i<180;++i) {
        char filename[40];std::snprintf(filename,sizeof(filename),"/frames/frame-%03d.png",i);
        auto image=cv::imread(assets+filename);
        if(image.empty()) throw std::runtime_error("Generated frame missing");
        const auto start=std::chrono::steady_clock::now();
        const auto p=nncourse::process(image,net,config,threshold);
        const double ms=std::chrono::duration<double,std::milli>(std::chrono::steady_clock::now()-start).count();
        ++frames;valid+=p.roi_valid;classified+=p.roi_valid&&p.classification.accepted;
        table<<i<<','<<p.roi_valid<<','<<(p.roi_valid&&p.classification.accepted)<<',';
        if(p.roi_valid&&p.classification.accepted)table<<p.classification.id;
        table<<','<<p.classification.score<<','<<p.has_pose<<',';
        if(p.has_pose)table<<p.tvec.at<double>(2);
        table<<','<<ms<<'\n';
        last=lab::mosaic({{"Original",image},{"Detection + digit + PnP",p.annotated},{"Perspective ROI",p.roi}});
        if(!headless) {
          cv::imshow(window,last);
          int key=cv::waitKey(std::max(1,parser.get<int>("value")))&255;
          if(key==' ')do{key=cv::waitKey(30)&255;}while(key!=' '&&key!='q'&&key!=27&&!lab::closed(window));
          if(key=='q'||key==27||lab::closed(window))break;
        }
      }
      results<<"frames"<<frames<<"roi_frames"<<valid<<"classified_frames"<<classified;
      cv::imwrite(output+"/overview.png",last);
      if(!headless&&!lab::closed(window)){cv::imshow(window,last);while(true){int k=cv::waitKey(30)&255;if(k=='q'||k==27||lab::closed(window))break;}}
    } else throw std::runtime_error("Unknown mode");
    results.release();
    std::cout<<"Saved: "<<output<<std::endl;
  } catch(const std::exception& e){std::cerr<<e.what()<<std::endl;return 1;}
}
